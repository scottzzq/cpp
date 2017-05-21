#include "Raft.h"
#include "Storage.h"
#include "Progress.h"
#include "tikv_common.h"
#include "RaftLog.h"
#include "Progress.h"
#include <limits.h>
#include <cstdlib>

// CAMPAIGN_PRE_ELECTION represents the first phase of a normal election when
// Config.pre_vote is true.
const std::string CAMPAIGN_PRE_ELECTION = "CampaignPreElection";
// CAMPAIGN_ELECTION represents a normal (time-based) election (the second phase
// of the election when Config.pre_vote is true).
const std::string CAMPAIGN_ELECTION  = "CampaignElection";
// CAMPAIGN_TRANSFER represents the type of leader transfer.
const std::string CAMPAIGN_TRANSFER = "CampaignTransfer";

bool operator != (const SoftState& lhs, const SoftState& rhs){
	return lhs.leader_id != rhs.leader_id ||
		lhs.raft_state == rhs.raft_state;
}

bool operator == (const SoftState& lhs, const SoftState& rhs){
	return lhs.leader_id == rhs.leader_id && 
		lhs.raft_state == rhs.raft_state;
}

eraftpb::Message new_message(uint64_t to, eraftpb::MessageType field_type, uint64_t from){
	eraftpb::Message m;
	m.set_to(to);
	if (from != INVALID_ID) {
		m.set_from(from);
	}
	m.set_msg_type(field_type);
	return m;
}

Progress* new_progress(uint64_t next_idx, uint64_t ins_size){
	return new Progress(next_idx, ins_size);
}

// vote_resp_msg_type maps vote and pre_vote message types to their correspond responses.
eraftpb::MessageType vote_resp_msg_type(eraftpb::MessageType t){
	switch (t){
		case eraftpb::MessageType::MsgRequestVote:
			return eraftpb::MessageType::MsgRequestVoteResponse;
		case eraftpb::MessageType::MsgRequestPreVote:
			return eraftpb::MessageType::MsgRequestPreVoteResponse;
		default:
			LOG_FATAL << "Not a vote message: " << t;
			exit(1);
			std::abort();
	}
}

Raft::Raft(RaftConfig conf, Storage* storage){
	this->storage_ = storage;
	this->raft_log = new RaftLog(storage);

	std::vector<uint64_t> peers = conf.peers;

	RaftState rs = storage->initial_state();
	if (rs.conf_state.nodes_size() != 0){
		if (!peers.empty()){
			LOG_ERROR << this->tag << " cannot specify both new(peers) and ConfState.Nodes";
			return;
		}

		for(int i = 0; i < rs.conf_state.nodes_size(); ++i){
			peers.push_back(rs.conf_state.nodes(i));
		}
	}

	this->id = conf.id;
	this->leader_id = 0;
	this->lead_transferee = 0;
	this->state = StateRole::Follower;

	this->vote = 0;
	this->term = 0;

	this->votes.clear();
	this->msgs.clear();

	this->pending_conf = false;

	this->max_inflight = conf.max_inflight_msgs;
	this->max_msg_size = conf.max_size_per_msg;

	this->check_quorum = conf.check_quorum;
	this->pre_vote = conf.pre_vote;

	this->election_elapsed = 0;
	this->heartbeat_elapsed = 0;
	this->randomized_election_timeout = 0;

	this->heartbeat_timeout = conf.heartbeat_tick;
	this->election_timeout = conf.election_tick;

	this->become_follower(1, INVALID_ID);

	LOG_INFO << "My id is:" << this->id;

	//为集群中每一个peer，先设置一个进度
	for(size_t i = 0; i < peers.size(); ++i){
		this->prs.insert(std::make_pair(peers[i], new_progress(1, this->max_inflight)));
		LOG_INFO << "add " << peers[i] << " to prs";
	}

	//从Storage获取到的HardState不为空的话，需要load
	if (rs.hard_state.SerializeAsString() != eraftpb::HardState().SerializeAsString()){
		this->load_state(rs.hard_state);
	}

	//apply to
	if (conf.applied > 0) {
		this->raft_log->applied_to(conf.applied);
	}
}

Raft::~Raft(){
}

SoftState Raft::soft_state(){
	SoftState ss;
	ss.leader_id = this->leader_id;
	ss.raft_state = this->state;
	return ss;
}

eraftpb::HardState Raft::hard_state(){
	auto hs = eraftpb::HardState();
	hs.set_term(this->term);
	hs.set_vote(this->vote);
	hs.set_commit(this->raft_log->committed);
	return hs;
}

// send persists state to stable storage and then sends to its mailbox.
void Raft::send(eraftpb::Message& m) {
	LOG_INFO << "send Raft MSG:" << m.DebugString();
	//设置消息源id
	m.set_from(this->id);
	//如果是投票的消息的话，term一定不为0，需要提前设置message的term，否则panic
	if (m.msg_type() == eraftpb::MessageType::MsgRequestVote ||
			m.msg_type() == eraftpb::MessageType::MsgRequestPreVote) {
		if (m.term() == 0) {
			// Pre-vote RPCs are sent at a term other than our actual term, so the code
			// that sends these messages is responsible for setting the term.
			char log[1024];
			snprintf(log, sizeof(log), "%s term should be set when sending %u",
					this->tag.c_str(),
					m.msg_type());
			LOG_FATAL << log;
			exit(1);
		}
		//不是投票的其他消息，不允许提前设置term，否则panic
	}else{
		if (m.term() != 0) {
			char log[1024];
			snprintf(log, sizeof(log), "%s term should not be set when sending %u(was %lu)",
					this->tag.c_str(),
					m.msg_type(),
					m.term());
			LOG_FATAL << log;
			exit(1);
		}
		// do not attach term to MsgPropose, MsgReadIndex
		// proposals are a way to forward to the leader and
		// should be treated as local message.
		// MsgReadIndex is also forwarded to leader.
		// MsgPropose & MsgReadIndex是直接转发给leader的，相当于local消息，不用设置term
		if (m.msg_type() != eraftpb::MessageType::MsgPropose &&
				m.msg_type() != eraftpb::MessageType::MsgReadIndex ){
			m.set_term(this->term);
		}
	}
	this->msgs.push_back(m);
}

void Raft::prepare_send_entries(eraftpb::Message& m, uint64_t to, uint64_t t, std::vector<eraftpb::Entry>& ents) {
	std::map<uint64_t, Progress*>::const_iterator it = this->prs.find(to);
	Progress* pr = NULL;
	if (it != this->prs.end()){
		pr = it->second;
		m.set_msg_type(eraftpb::MessageType::MsgAppend);
		m.set_index(pr->next_idx - 1);
		m.set_log_term(t);
		m.set_commit(this->raft_log->committed);
		LOG_INFO << "send MsgAppend to:[" << to << "]" << "log_index:[" << pr->next_idx - 1 << "]" << "log_term:[" << t << "]";

		for (size_t i = 0; i < ents.size(); ++i){
			eraftpb::Entry* en = m.add_entries();
			en->CopyFrom(ents[i]);
		}

		if (m.entries_size() > 0) {
			switch (pr->state){
				case ProgressState::Replicate:
					{
						uint64_t last_idx = 0;
						last_idx = m.entries(m.entries_size() - 1).index();
						pr->optimistic_update(last_idx);
						pr->ins.add(last_idx);
					}
					break;
				case ProgressState::Probe:
					{
						pr->pause();
					}
					break;
				default:
					LOG_INFO << this->tag << " is sending append in unhandled state " << pr->state;
					exit(1);
					std::abort();
					break;
			}
		}
	}
}
// send_append sends RPC, with entries to the given peer.
void Raft::send_append(uint64_t to) {
	std::map<uint64_t, Progress*>::const_iterator it = this->prs.find(to);
	if (it != this->prs.end()){
		Progress* pr = it->second;
		uint64_t t;
		int ret = this->raft_log->term(pr->next_idx - 1, t);
		assert(ret == 0);

		std::vector<eraftpb::Entry> entries = this->raft_log->entries(pr->next_idx, this->max_msg_size);
		LOG_INFO << "send_append to:" << it->first << " pr->next_idx:" << pr->next_idx << " entries size:" << entries.size(); 

		auto m = eraftpb::Message();
		m.set_to(to);
		//if term.is_err() || ents.is_err() {
		//	// send snapshot if we failed to get term or entries
		//	if !self.prepare_send_snapshot(&mut m, to) {
		//		return;
		//	}
		//} else {
		this->prepare_send_entries(m, to, t, entries);
		//}
		this->send(m);
		++it;
	}
}

/////////////////////heartbeat相关消息/////////////////////////
// send_heartbeat sends an empty MsgAppend
//为其他peer发送HeatBeat消息，消息体中携带有commit信息
//取matched和committed最小的一个
void Raft::send_heartbeat(uint64_t to, std::string ctx) {
	// Attach the commit as min(to.matched, self.raft_log.committed).
	// When the leader sends out heartbeat message,
	// the receiver(follower) might not be matched with the leader
	// or it might not have all the committed entries.
	// The leader MUST NOT forward the follower's commit to
	// an unmatched index.
	LOG_INFO << "to:" << to << " matched:" << this->prs[to]->matched << " committed:" << this->raft_log->committed;
	auto m = eraftpb::Message();
	m.set_to(to);
	m.set_msg_type(eraftpb::MessageType::MsgHeartbeat);
	uint64_t commit = std::min(this->prs[to]->matched, this->raft_log->committed);
	m.set_commit(commit);
	if (ctx != ""){
		m.set_context(ctx);
	}
	this->send(m);
}

// bcast_append sends RPC, with entries to all peers that are not up-to-date
// according to the progress recorded in r.prs.
void Raft::bcast_append() {
	// TODO: avoid copy
	std::vector<uint64_t> ids;
	std::map<uint64_t, Progress*>::const_iterator it = this->prs.begin();
	while (it != this->prs.end()){
		ids.push_back(it->first);
		++it;
	}
	for(size_t i = 0; i < ids.size(); ++i){
		if (ids[i] == this->id) {
			continue;
		}
		this->send_append(ids[i]);
	}
}

// bcast_heartbeat sends RPC, without entries to all the peers.
void Raft::bcast_heartbeat() {
	this->bcast_heartbeat_with_ctx("");
}

void Raft::bcast_heartbeat_with_ctx(std::string ctx) {
	// TODO: avoid copy
	std::vector<uint64_t> ids;
	std::map<uint64_t, Progress*>::const_iterator it = this->prs.begin();
	while (it != this->prs.end()){
		ids.push_back(it->first);
		++it;
	}

	for (size_t i = 0; i < ids.size(); ++i){
		if (ids[i] == this->id) {
			continue;
		}
		this->send_heartbeat(ids[i], ctx);
	}
}

bool Raft::maybe_commit() {
	// TODO: optimize
	std::vector<uint64_t> mis;
	mis.reserve(this->prs.size());
	for (auto& kv : this->prs) {
		mis.push_back(kv.second->matched);
	}
	std::sort(mis.begin(), mis.end());
	//// reverse sort
	uint64_t mci = mis[this->quorum() - 1];
	this->raft_log->maybe_commit(mci, this->term);
	LOG_INFO << "maybe_commit:" << mci;
	return true;
}

void Raft::reset(uint64_t t){
	if (this->term != t) {
		this->term = t;
		this->vote = INVALID_ID;
	}
	this->leader_id = INVALID_ID;
	this->reset_randomized_election_timeout();
	this->election_elapsed = 0;
	this->heartbeat_elapsed = 0;
	this->votes.clear();

	uint64_t last_index = this->raft_log->last_index();
	uint64_t self_id = this->id;
	std::map<uint64_t, Progress*>::iterator it = this->prs.begin();
	while (it != this->prs.end()){
		uint64_t cur_id = it->first;
		Progress* p = it->second;
		p->next_idx = last_index + 1;
		if (cur_id == self_id){
			p->matched = last_index;
		}
		++it;
	}
}

void Raft::append_entry(std::vector<eraftpb::Entry>& es) {
	uint64_t li = this->raft_log->last_index();
	for (size_t i = 0; i < es.size(); ++i){
		es[i].set_term(this->term);
		es[i].set_index(li + 1 + i);
	}
	this->raft_log->append(es);
	auto it = this->prs.find(this->id);
	if (it != this->prs.end()){
		it->second->maybe_update(this->raft_log->last_index());
		// Regardless of maybe_commit's return, our caller will call bcastAppend.
		this->maybe_commit();
	}
}

void Raft::tick(){
	LOG_INFO << "Raft::tick ";
	switch (this->state) {
		case StateRole::Follower:
		case StateRole::PreCandidate:
		case StateRole::Candidate:
			this->tick_election();
			break;
		case StateRole::Leader:
			this->tick_heartbeat();
	}
}

void Raft::tick_election(){
	LOG_INFO << "Raft::tick_election";
	this->election_elapsed += 1;
	if (this->promotable() && this->pass_election_timeout()) {
		this->election_elapsed = 0;
		auto m = new_message(INVALID_ID, eraftpb::MessageType::MsgHup, this->id);
		this->step(m);
	}
}

void Raft::tick_heartbeat(){
	LOG_INFO << "Raft::tick_heartbeat";
	this->election_elapsed += 1;
	this->heartbeat_elapsed += 1;
	if (this->election_elapsed >= this->randomized_election_timeout){
		this->election_elapsed = 0;
		if (this->check_quorum){
			eraftpb::Message m = new_message(INVALID_ID, eraftpb::MessageType::MsgCheckQuorum, this->id);
			this->step(m);
		}
	}
	if (this->state != StateRole::Leader) {
		return;
	}

	//心跳时间到，需要发送MessageType::MsgBeat，调用step处理
	if (this->heartbeat_elapsed >= this->heartbeat_timeout) {
		this->heartbeat_elapsed = 0;
		auto m = new_message(INVALID_ID, eraftpb::MessageType::MsgBeat, this->id);
		this->step(m);
	}
}

void Raft::become_follower(uint64_t term, uint64_t leader_id){
	this->reset(term);
	this->leader_id = leader_id;
	this->state = StateRole::Follower;
	LOG_INFO << this->tag << " became follower at term " << this->term;
}

void Raft::become_candidate(){
	assert(this->state != StateRole::Leader);
	uint64_t term = this->term + 1;

	this->reset(term);
	uint64_t id = this->id;
	this->vote = id;
	this->state = StateRole::Candidate;
	LOG_INFO << this->tag << " became candidate at term " << this->term;
}

void Raft::become_leader(){
	assert(this->state != StateRole::Follower);
	uint64_t vote_term = this->term;
	this->reset(vote_term);
	this->leader_id = id;
	this->state = StateRole::Leader;
	uint64_t begin = this->raft_log->committed + 1;
	std::vector<eraftpb::Entry> ents = this->raft_log->entries(begin, NO_LIMIT);
	auto nconf = this->num_pending_conf(ents);
	if (nconf > 1) {
		//TODO
		assert(nconf <= 1);
		//panic!("{} unexpected double uncommitted config entry", self.tag);
	}
	if (nconf == 1) {
		this->pending_conf = true;
	}
	std::vector<eraftpb::Entry> ens;
	ens.push_back(eraftpb::Entry());
	this->append_entry(ens);
	LOG_INFO << this->tag << " became leader at term " << this->term;
}

void Raft::become_pre_candidate() {
	assert(this->state != StateRole::Leader);
	// Becoming a pre-candidate changes our state.
	// but doesn't change anything else. In particular it does not increase
	// self.term or change self.vote.
	this->state = StateRole::PreCandidate;
	LOG_INFO << this->tag << " became pre-candidate at term " << this->term;
}

uint64_t Raft::num_pending_conf(std::vector<eraftpb::Entry>& ents) {
	uint64_t count = 0;
	for (size_t i = 0; i < ents.size(); ++i){
		if (ents[i].entry_type() == eraftpb::EntryType::EntryConfChange){
			count += 1;
		}
	}
	return count;
}
void Raft::campaign(std::string campaign_type) {
	LOG_INFO << "Raft::campaign:" << campaign_type;

	auto vote_msg = eraftpb::MessageType::MsgRequestVote;
	auto vote_term = this->term;
	if (campaign_type == CAMPAIGN_PRE_ELECTION) {
		this->become_pre_candidate();
		// Pre-vote RPCs are sent for next term before we've incremented self.term.
		vote_msg = eraftpb::MessageType::MsgRequestPreVote;
		vote_term = this->term + 1;
	} else {
		this->become_candidate();
		vote_msg = eraftpb::MessageType::MsgRequestVote;
		vote_term = this->term;
	}

	uint64_t poll_res = this->poll(this->id, true);
	if (this->quorum() == poll_res) {
		if (campaign_type == CAMPAIGN_PRE_ELECTION) {
			this->campaign(CAMPAIGN_ELECTION);
		} else {
			this->become_leader();
		}
		return;
	}

	std::vector<uint64_t> ids;
	std::map<uint64_t, Progress*>::const_iterator it = this->prs.begin();
	while (it != this->prs.end()){
		ids.push_back(it->first);
		++it;
	}

	for (size_t i = 0; i < ids.size(); ++i){
		LOG_INFO << "campaign:" << this->id << " " << ids[i];
		if (this->id == ids[i]){
			continue;
		}
		LOG_INFO << this->tag << " [logterm: " << this->raft_log->last_term() << 
			", index: " << this->raft_log->last_index() 
			<< "] sent vote request to " << ids[i] << " at term " << this->term;
		auto m = new_message(ids[i], vote_msg, INVALID_ID);
		m.set_term(vote_term);
		m.set_index(this->raft_log->last_index());
		m.set_log_term(this->raft_log->last_term());
		if (campaign_type == CAMPAIGN_TRANSFER){
			m.set_context(campaign_type);
		}
		this->send(m);
	}
}

uint64_t Raft::poll(uint64_t id, bool v){
	if (v){
		LOG_INFO << this->tag << " received vote from " << id << " at term " << this->term;
	} else {
		LOG_INFO << this->tag << " received vote rejection from " << id << " at term " << this->term;
	}
	this->votes.insert(std::make_pair(id, v));
	uint64_t count = 0;
	std::map<uint64_t, bool>::const_iterator it = this->votes.begin();
	while (it != this->votes.end())	{
		if (it->second == true){
			count += 1;
		}
		++it;
	}
	return count;
}

void Raft::step(eraftpb::Message m){
	if (m.term() == 0) {
		// local message
	} else if (m.term() > this->term){
		//收到消息的term比当前的term要大
		uint64_t leader_id;
		//如果是MsgRequestVote MsgRequestPreVote投票类型消息,判断当前lease是否有效，是否强制
		if (m.msg_type() == eraftpb::MessageType::MsgRequestVote ||
				m.msg_type() == eraftpb::MessageType::MsgRequestPreVote) {
			// Transfer leader是强制,否则非强制
			bool force = (m.context() == CAMPAIGN_TRANSFER);
			// lease 是否过期
			bool in_lease = this->check_quorum && this->leader_id != INVALID_ID &&
				this->election_elapsed < this->election_timeout;
			//不是强制，并且在lease有效
			if (!force && in_lease) {
				// if a server receives ReqeustVote request within the minimum election
				// timeout of hearing from a current leader, it does not update its term
				// or grant its vote
				char log[1024];
				memset(log, 0, sizeof(log));

				snprintf(log, sizeof(log), "%s [logterm: %lu, index: %lu, vote: %lu] ignored %d vote from \
						%lu [logterm: %lu, index: %lu] at term %lu: lease is not expired \
						(remaining ticks: %lu)",
						this->tag.c_str(),
						this->raft_log->last_term(),
						this->raft_log->last_index(),
						this->vote,
						m.msg_type(),
						m.from(),
						m.log_term(),
						m.index(),
						this->term,
						this->election_timeout - this->election_elapsed);
				LOG_INFO << log;
				return;
			}
			leader_id = INVALID_ID;
		} else {
			leader_id = m.from();
		}

		//如果是MsgRequestPreVote，或者是MsgRequestPreVoteResponse类型并且没有reject,就不需要切换成Follower
		//否则需要主动切换成Follower
		if (m.msg_type() == eraftpb::MessageType::MsgRequestPreVote ||
				(m.msg_type() == eraftpb::MessageType::MsgRequestPreVoteResponse && !m.reject())) {
			// For a pre-vote request:
			// Never change our term in response to a pre-vote request.
			//
			// For a pre-vote response with pre-vote granted:
			// We send pre-vote requests with a term in our future. If the
			// pre-vote is granted, we will increment our term when we get a
			// quorum. If it is not, the term comes from the node that
			// rejected our vote so we should become a follower at the new
			// term.
		} else {
			char log[1024];
			memset(log, 0, sizeof(log));

			snprintf(log, sizeof(log), "%s [term: %lu] received a %d message with higher term from %lu [term: %lu]",
					this->tag.c_str(),
					this->term,
					m.msg_type(),
					m.from(),
					m.term());
			LOG_INFO << log;
			this->become_follower(m.term(), leader_id);
		}	
	} else if (m.term() < this->term){
		//1.其他leader发送的消息由于网络延迟，到达时间晚,回复之后，leader会增加term
		//2.网络分区，当前节点多次发起投票，但是都没有赢得投票，不能成为leader,同时也不能加入原来的group中,忽略
		if (this->check_quorum &&
				(m.msg_type() == eraftpb::MessageType::MsgHeartbeat ||
				 m.msg_type() == eraftpb::MessageType::MsgAppend)) {
			// We have received messages from a leader at a lower term. It is possible
			// that these messages were simply delayed in the network, but this could
			// also mean that this node has advanced its term number during a network
			// partition, and it is now unable to either win an election or to rejoin
			// the majority on the old term. If checkQuorum is false, this will be
			// handled by incrementing term numbers in response to MsgVote with a higher
			// term, but if checkQuorum is true we may not advance the term on MsgVote and
			// must generate other messages to advance the term. The net result of these
			// two features is to minimize the disruption caused by nodes that have been
			// removed from the cluster's configuration: a removed node will send MsgVotes
			// which will be ignored, but it will not receive MsgApp or MsgHeartbeat, so it
			// will not create disruptive term increases
			eraftpb::Message to_send = new_message(m.from(), eraftpb::MessageType::MsgAppendResponse, INVALID_ID);
			this->send(to_send);
		} else {
			// ignore other cases
			char log[1024];
			memset(log, sizeof(log), 0);
			snprintf(log, sizeof(log), "%s [term: %lu] ignored a %d message with lower term from %lu [term: %lu]",
					tag.c_str(),
					this->term,
					m.msg_type(),
					m.from(),
					m.term());
			LOG_INFO << log;
		}
		return;
	}
	switch (m.msg_type()){
		case eraftpb::MessageType::MsgHup:
			{
				if (this->state != StateRole::Leader) {
					std::vector<eraftpb::Entry> ents = this->raft_log
						->slice(this->raft_log->applied + 1,
								this->raft_log->committed + 1,
								NO_LIMIT);
					uint64_t n = this->num_pending_conf(ents);
					if (n != 0 && this->raft_log->committed > this->raft_log->applied ){
						LOG_WARN << this->id << " cannot campaign at term " << this->term 
							<< " since there are still " << n << " pending configuration changes to apply";
					}
					LOG_INFO << this->tag << " is starting a new election at term " << this->term;
					if (this->pre_vote) {
						this->campaign(CAMPAIGN_PRE_ELECTION);
					} else {
						this->campaign(CAMPAIGN_ELECTION);
					}
				}else{
					LOG_DEBUG << this->tag << " ignoring MsgHup because already leader";
				}
				break;
			}
		case eraftpb::MessageType::MsgRequestVote:
		case eraftpb::MessageType::MsgRequestPreVote:
			{
				// The m.get_term() > self.term clause is for MsgRequestPreVote. For MsgRequestVote
				// m.get_term() should always equal self.term
				if ((this->vote == INVALID_ID || m.term() > this->term ||
							this->vote == m.from()) &&
						this->raft_log->is_up_to_date(m.index(), m.log_term())) {
					this->log_vote_approve(m);
					auto to_send =
						new_message(m.from(), vote_resp_msg_type(m.msg_type()), INVALID_ID);
					to_send.set_reject(false);
					this->send(to_send);
					if (m.msg_type() == eraftpb::MessageType::MsgRequestVote) {
						// Only record real votes.
						this->election_elapsed = 0;
						this->vote = m.from();
					}
				} else {
					this->log_vote_reject(m);
					auto to_send =
						new_message(m.from(), vote_resp_msg_type(m.msg_type()), INVALID_ID);
					to_send.set_reject(true);
					this->send(to_send);
				}
				break;
			}
		default:
			{
				switch(this->state){
					case StateRole::Candidate:
					case StateRole::PreCandidate:
						this->step_candidate(m);
						break;
					case StateRole::Follower:
						this->step_follower(m);
						break;
					case StateRole::Leader:
						this->step_leader(m);
						break;
					default:
						break;
				}	
			}
	}
}

void Raft::step_candidate(eraftpb::Message m){
	LOG_INFO << "Raft::step_candidate:" << m.DebugString();

	uint64_t cur_term = this->term;
	switch(m.msg_type()){
		case eraftpb::MessageType::MsgPropose :
			{
				LOG_INFO << this->tag << " no leader at term " << this->term << "; dropping proposal";
			}
			break;
		case eraftpb::MessageType::MsgAppend:
			{
				this->become_follower(term, m.from());
				this->handle_append_entries(m);
			}
			break;
		case eraftpb::MessageType::MsgHeartbeat:
			{
				this->become_follower(term, m.from());
				this->handle_heartbeat(m);
			}
			break;
		case eraftpb::MessageType::MsgSnapshot:
			{
				this->become_follower(term, m.from());
				//this->handle_snapshot(m);
			}
			break;
		case eraftpb::MessageType::MsgRequestPreVoteResponse:
		case eraftpb::MessageType::MsgRequestVoteResponse:
			{
				// Only handle vote responses corresponding to our candidacy (while in
				// state Candidate, we may get stale MsgPreVoteResp messages in this term from
				// our pre-candidate state).
				if ((this->state == StateRole::PreCandidate &&
							m.msg_type() != eraftpb::MessageType::MsgRequestPreVoteResponse) ||
						(this->state == StateRole::Candidate &&
						 m.msg_type() != eraftpb::MessageType::MsgRequestVoteResponse)) {
					return;
				}

				uint64_t gr = this->poll(m.from(), !m.reject());
				LOG_INFO << this->tag << " [quorum:" << this->quorum() <<"] has received "<< gr 
					<<" " << m.msg_type() <<" votes and " << this->votes.size() - gr <<" vote rejections";
				if (this->quorum() == gr) {
					if (this->state == StateRole::PreCandidate) {
						this->campaign(CAMPAIGN_ELECTION);
					} else {
						this->become_leader();
						this->bcast_append();
					}
				} else if (this->quorum() == this->votes.size() - gr) {
					this->become_follower(term, INVALID_ID);
				}
			}
			break;
		case eraftpb::MessageType::MsgTimeoutNow:
			{
				LOG_INFO << this->tag << " [term " << this->term << " state " 
					<< this->state << "] ignored MsgTimeoutNow from " << m.from();
			}
			break;
		default: 
			{
			}
	}
}
void Raft::step_follower(eraftpb::Message& m){
	switch(m.msg_type()) {
		case eraftpb::MessageType::MsgPropose:
			{
				if (this->leader_id == INVALID_ID) {
					LOG_INFO << this->tag << " no leader at term " << this->term << "; dropping proposal";
					return;
				}
				m.set_to(this->leader_id);
				this->send(m);
			}
			break;
		case eraftpb::MessageType::MsgAppend: 
			{
				this->election_elapsed = 0;
				this->leader_id = m.from();
				this->handle_append_entries(m);
			}
			break;
		case eraftpb::MessageType::MsgHeartbeat:
			{
				this->election_elapsed = 0;
				this->leader_id = m.from();
				this->handle_heartbeat(m);
			}
			break;
		case eraftpb::MessageType::MsgSnapshot:
			//{
			//	self.election_elapsed = 0;
			//	self.leader_id = m.get_from();
			//	self.handle_snapshot(m);
			//}
			break;
		case eraftpb::MessageType::MsgRequestVote:
			{
				auto t = eraftpb::MessageType::MsgRequestVoteResponse;
				if ((this->vote == INVALID_ID || this->vote == m.from()) &&
						this->raft_log->is_up_to_date(m.index(), m.log_term())) {
					this->log_vote_approve(m);
					this->election_elapsed = 0;
					this->vote = m.from();
					auto to_send = new_message(m.from(), t, INVALID_ID);
					to_send.set_reject(false);
					this->send(to_send);
				} else {
					this->log_vote_reject(m);
					auto to_send = new_message(m.from(), t, INVALID_ID);
					to_send.set_reject(true);
					this->send(to_send);
				}
			}
			break;
		case eraftpb::MessageType::MsgTransferLeader:
			{
				if (this->leader_id == INVALID_ID) {
					LOG_INFO << this->tag << " no leader at term " << this->term << "; dropping leader transfer msg";
					return;
				}
				m.set_to(this->leader_id);
				this->send(m);
			}
			break;
		case eraftpb::MessageType::MsgTimeoutNow:
			{
				char log[1024];
				snprintf(log, sizeof(log), "%s [term %lu] received MsgTimeoutNow from %lu and starts an election to \
						get leadership.",
						this->tag.c_str(),
						this->term,
						m.from());
				this->campaign(CAMPAIGN_TRANSFER);
			}
			break;
		default:
			{
			}
	}
}

void Raft::step_leader(eraftpb::Message m){
	switch(m.msg_type()){
		case eraftpb::MessageType::MsgBeat:
			{
				this->bcast_heartbeat();
				return;
			}
		case eraftpb::MessageType::MsgCheckQuorum:
			{
				if (!this->check_quorum_active()) {
					LOG_WARN << this->tag << " stepped down to follower since quorum is not active";
					auto cur_term = this->term;
					this->become_follower(cur_term, INVALID_ID);
				}
				return;
			}
		case eraftpb::MessageType::MsgPropose: 
			{
				if (m.entries().size() == 0) {
					LOG_FATAL << this->tag << " stepped empty MsgProp";;
				}
				if (this->prs.count(this->id) == 0){
					// If we are not currently a member of the range (i.e. this node
					// was removed from the configuration while serving as leader),
					// drop any new proposals.
					return;
				}
				if (this->lead_transferee != 0) {
					char log[1024];
					snprintf(log, sizeof(log), "%s [term %lu] transfer leadership to %lu is in progress; dropping \
							proposal",
							this->tag.c_str(),
							this->term,
							this->lead_transferee);
					return;
				}

				for(int i = 0; i < m.entries().size(); ++i){
					auto e = m.entries(i);
					if (e.entry_type() == eraftpb::EntryType::EntryConfChange) {
						if (this->pending_conf) {
							LOG_INFO << "propose conf " << e.DebugString() << "ignored since pending unapplied configuration";
							e = eraftpb::Entry();
							e.set_entry_type(eraftpb::EntryType::EntryNormal);
						}
						this->pending_conf = true;
					}
				}

				std::vector<eraftpb::Entry> ens;
				for (auto& m_e: m.entries()){
					ens.push_back(m_e);
				}
				this->append_entry(ens);
				this->bcast_append();
				return;
			}
			break;
		case eraftpb::MessageType::MsgUnreachable: 
			{
				auto pr = this->prs[m.from()];
				// During optimistic replication, if the remote becomes unreachable,
				// there is huge probability that a MsgAppend is lost.
				if (pr->state == ProgressState::Replicate) {
					pr->become_probe();
				}
				LOG_INFO << this->tag << " failed to send message to " << m.from() << " because it is unreachable " << pr;
			}
		default:
			{
			}	
	}
	bool send_append = false;
	bool maybe_commit = false;
	bool old_paused = false;
	eraftpb::Message to_send;

	this->check_message_with_progress(m,
			send_append,
			old_paused,
			maybe_commit,
			to_send);
	LOG_INFO << "step_leader:" << maybe_commit;
	if (maybe_commit) {
		if (this->maybe_commit()) {
			this->bcast_append();
		} else if (old_paused) {
			// update() reset the wait state on this node. If we had delayed sending
			// an update before, send it now.
			send_append = true;
		}
	}

	if (send_append) {
		this->send_append(m.from());
	}
	if (to_send.to() !=INVALID_ID) {
		this->send(to_send);
	}
}
// TODO: revoke pub when there is a better way to test.
void Raft::handle_append_entries(eraftpb::Message& m) {
	LOG_INFO << "handle_append_entries:" << m.DebugString();
	if (m.index() < this->raft_log->committed) {
		auto to_send = eraftpb::Message();
		to_send.set_to(m.from());
		to_send.set_msg_type(eraftpb::MessageType::MsgAppendResponse);
		to_send.set_index(this->raft_log->committed);
		this->send(to_send);
		return;
	}
	auto to_send = eraftpb::Message();
	to_send.set_to(m.from());
	to_send.set_msg_type(eraftpb::MessageType::MsgAppendResponse);
	uint64_t idx = this->raft_log->maybe_append(m.index(),
			m.log_term(),
			m.commit(),
			m.entries());
	if (idx != 0){
		to_send.set_index(idx);
		this->send(to_send);
	}else{
		uint64_t t;
		int ret = this->raft_log->term(m.index(), t);
		assert(ret == 0);
		char log[1024];
		snprintf(log, sizeof(log), "%s [logterm: %lu, index: %lu] rejected msgApp [logterm: %lu, index: %lu] from %lu",
				this->tag.c_str(),
				t,
				m.index(),
				m.log_term(),
				m.index(),
				m.from());
		LOG_INFO << log;
		to_send.set_index(m.index());
		to_send.set_reject(true);
		to_send.set_reject_hint(this->raft_log->last_index());
		this->send(to_send);
	}
}

// TODO: revoke pub when there is a better way to test.
void Raft::handle_heartbeat(eraftpb::Message m) {
	this->raft_log->commit_to(m.commit());
	eraftpb::Message to_send;
	to_send.set_to(m.from());
	to_send.set_msg_type(eraftpb::MessageType::MsgHeartbeatResponse);
	to_send.set_context(m.context());
	this->send(to_send);
}


void Raft::add_node(uint64_t node_id){
	this->pending_conf = false;
	if (this->prs.find(node_id) != this->prs.end()){
		// Ignore any redundant addNode calls (which can happen because the
		// initial bootstrapping entries are applied twice).
		return;
	}

	uint64_t last_index = this->raft_log->last_index();
	this->set_progress(node_id, 0, last_index + 1);
}         

void Raft::remove_node(uint64_t node_id){
	this->del_progress(node_id);
	this->pending_conf = false;
	// do not try to commit or abort transferring if there is no nodes in the cluster.
	if (this->prs.empty()) {
		return;
	}

	// The quorum size is now smaller, so see if any pending entries can
	// be committed.
	if (maybe_commit()){
		this->bcast_append();
	}
	// If the removed node is the lead_transferee, then abort the leadership transferring.
	if (this->state == StateRole::Leader && this->lead_transferee == node_id) {
		this->abort_leader_transfer();
	}
}

void Raft::set_progress(uint64_t node_id, uint64_t matched, uint64_t next_idx) {
	auto p = new_progress(next_idx, this->max_inflight);
	p->matched = matched;
	this->prs.insert(std::make_pair(node_id, p));
}

void  Raft::del_progress(uint64_t node_id) {
	auto it = this->prs.find(node_id);
	assert(it != this->prs.end());
	this->prs.erase(it);
}

void Raft::load_state(eraftpb::HardState hs) {
	if (hs.commit() < this->raft_log->committed ||
			hs.commit() > this->raft_log->last_index()) {
		LOG_ERROR << "hs.commit {} is out of range [{}, {}]";
	}
	this->raft_log->committed = hs.commit();
	this->term = hs.term();
	this->vote = hs.vote();
	LOG_INFO << "Raft::load_state, term:" << this->term << " vote: " << this->vote << " commit:" << hs.commit();
}

bool Raft::pass_election_timeout() {
	LOG_INFO << "this->election_elapsed:[" << this->election_elapsed << 
		"] this->randomized_election_timeout:[" << this->randomized_election_timeout << "]"; 
	return this->election_elapsed >= this->randomized_election_timeout;
}

void Raft::reset_randomized_election_timeout(){
	uint64_t prev_timeout = this->randomized_election_timeout;
	uint64_t timeout = this->election_timeout + (rand() % this->election_timeout);
	LOG_DEBUG << this->tag << " reset election timeout " << prev_timeout << "-> " << timeout << " at " << this->election_elapsed;
	this->randomized_election_timeout = timeout;
}

// check_quorum_active returns true if the quorum is active from
// the view of the local raft state machine. Otherwise, it returns
// false.
// check_quorum_active also resets all recent_active to false.
bool Raft::check_quorum_active() {
	uint64_t act = 0;
	for(auto& kv: this->prs){
		if (kv.first == this->id){
			act += 1;
			continue;
		}
		if (kv.second->recent_active){
			act += 1;
		}
		kv.second->recent_active = false;
	}
	return act >= this->quorum();
}

void Raft::log_vote_approve(eraftpb::Message& m) {
	char log[1024];
	memset(log, 0, sizeof(log));

	snprintf(log, sizeof(log), "log_vote_approve %s [logterm: %lu, index: %lu, vote: %lu] cast %d for %lu [logterm: %lu, index: %lu] \
			at term %lu",
			this->tag.c_str(),
			this->raft_log->last_term(),
			this->raft_log->last_index(),
			this->vote,
			m.msg_type(),
			m.from(),
			m.log_term(),
			m.index(),
			this->term);
	LOG_INFO << log;
}

void Raft::log_vote_reject(eraftpb::Message& m) {
	char log[1024];
	memset(log, 0, sizeof(log));

	snprintf(log, sizeof(log), "log_vote_reject %s [logterm: %lu, index: %lu, vote: %lu] rejected %d for %lu [logterm: %lu, index: %lu] \
			at term %lu",
			this->tag.c_str(),
			this->raft_log->last_term(),
			this->raft_log->last_index(),
			this->vote,
			m.msg_type(),
			m.from(),
			m.log_term(),
			m.index(),
			this->term);
	LOG_INFO << log;
}

void Raft::handle_append_response(eraftpb::Message& m,
		bool& old_paused,
		bool& send_append,
		bool& maybe_commit){
	auto it = this->prs.find(m.from());
	assert(it != this->prs.end());

	Progress* pr = it->second;
	pr->recent_active = true;
	if (m.reject()) {
		char log[1024];
		snprintf(log, sizeof(log), "%s received msgAppend rejection(lastindex: %lu) from %lu for index %lu",
				this->tag.c_str(),
				m.reject_hint(),
				m.from(),
				m.index());
		LOG_INFO << log;

		if (pr->maybe_decr_to(m.index(), m.reject_hint())) {
			LOG_INFO << this->tag << " decreased progress of " << m.from() <<"to " << pr;
			if (pr->state == ProgressState::Replicate) {
				pr->become_probe();
			}
			send_append = true;
		}
		return;
	}

	{
		old_paused = pr->is_paused();
		if (!pr->maybe_update(m.index())) {
			return;
		}
	}

	// Transfer leadership is in progress.
	if (this->lead_transferee != 0) {
		if (m.from() == lead_transferee && this->prs[m.from()]->matched == this->raft_log->last_index()) {
			LOG_INFO << this->tag << " sent MsgTimeoutNow to " << m.from() << " after received MsgAppResp";
			this->send_timeout_now(m.from());
		}
	}

	switch (pr->state) {
		case ProgressState::Probe:
			pr->become_replicate();
			break;
		case ProgressState::Snapshot:
			//if (!pr.maybe_snapshot_abort() ){
			//	return;
			//}
			//debug!("{} snapshot aborted, resumed sending replication messages to {} \
			//		[{:?}]",
			//		self.tag,
			//		m.get_from(),
			//		pr);
			//pr.become_probe();
			break;
		case ProgressState::Replicate:
			pr->ins.free_to(m.index());
			break;
	}
	maybe_commit = true;
	LOG_INFO << "handle_append_response:" << maybe_commit;
}

void Raft::handle_transfer_leader(eraftpb::Message& m){
	auto lead_transferee = m.from();
	auto last_lead_transferee = this->lead_transferee;
	char log[1024];
	if (last_lead_transferee != INVALID_ID){
		if (last_lead_transferee == lead_transferee) {
			snprintf(log, sizeof(log), "%s [term %lu] transfer leadership to %lu is in progress, ignores request \
					to same node %lu",
					this->tag.c_str(),
					this->term,
					lead_transferee,
					lead_transferee);
			LOG_INFO << log;
			return;
		}
		this->abort_leader_transfer();
		snprintf(log, sizeof(log), "%s [term %lu] abort previous transferring leadership to %lu",
				this->tag.c_str(),
				this->term,
				last_lead_transferee);
		LOG_INFO << log;
	}
	if (lead_transferee == this->id) {
		snprintf(log, sizeof(log), "%s is already leader. Ignored transferring leadership to self",
				this->tag.c_str());
		LOG_INFO << log;
		return;
	}
	// Transfer leadership to third party.
	snprintf(log, sizeof(log), "%s [term %lu] starts to transfer leadership to %lu",
			this->tag.c_str(),
			this->term,
			lead_transferee);
	LOG_INFO << log;
	// Transfer leadership should be finished in one electionTimeout
	// so reset r.electionElapsed.
	this->election_elapsed = 0;
	this->lead_transferee = lead_transferee;

	if (this->prs[m.from()]->matched == this->raft_log->last_index()){
		this->send_timeout_now(lead_transferee);
		LOG_INFO << this->tag << " sends MsgTimeoutNow to " << lead_transferee << " immediately as " << lead_transferee << "already has up-to-date log";
	} else {
		this->send_append(lead_transferee);
	}
}

void Raft::send_timeout_now(uint64_t to) {
	auto msg = new_message(to, eraftpb::MessageType::MsgTimeoutNow, INVALID_ID);
	this->send(msg);
}

/// check message's progress to decide which action should be taken.
void Raft::check_message_with_progress(eraftpb::Message& m,
		bool& send_append,
		bool& old_paused,
		bool& maybe_commit,
		eraftpb::Message& to_send){
	auto it = this->prs.find(m.from());
	if (it == this->prs.end()){
		LOG_INFO << this->tag << " no progress available for " << m.from();
		return;
	}

	switch (m.msg_type()) {
		case eraftpb::MessageType::MsgAppendResponse:
			this->handle_append_response(m, old_paused, send_append, maybe_commit);
			LOG_INFO << "check_message_with_progress:" << maybe_commit;
			break;
		case eraftpb::MessageType::MsgHeartbeatResponse:
			{
				auto it = this->prs.find(m.from());
				assert(it != this->prs.end());
				Progress* pr = it->second;
				pr->recent_active = true;
				pr->resume();

				// free one slot for the full inflights window to allow progress.
				if (pr->state == ProgressState::Replicate && pr->ins.full()) {
					pr->ins.free_first_one();
				}
				if (pr->matched < this->raft_log->last_index()) {
					send_append = true;
				}
			}
			//TODO Read
			break;
		case eraftpb::MessageType::MsgSnapStatus:
			//{
			//	if self.prs[&m.get_from()].state != ProgressState::Snapshot {
			//		return;
			//	}
			//	self.handle_snapshot_status(m);
			//}
			break;
		case eraftpb::MessageType::MsgUnreachable:
			{
				auto it = this->prs.find(m.from());
				assert(it != this->prs.end());

				Progress* pr = it->second;
				// During optimistic replication, if the remote becomes unreachable,
				// there is huge probability that a MsgAppend is lost.
				if (pr->state == ProgressState::Replicate) {
					pr->become_probe();
				}
				LOG_INFO << this->tag << " failed to send message to " << m.from() << " because it is unreachable " << pr;
			}
			break;
		case eraftpb::MessageType::MsgTransferLeader:
			{
				this->handle_transfer_leader(m);
			}
			break;
		default:
			break;
	}
}
