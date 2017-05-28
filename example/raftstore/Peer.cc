#include <stdint.h>
#include <string>
#include <deque>
#include <set>

#include "Peer.h"
#include "TiKVConf.h"
#include "RaftConfig.h"
#include "PeerStorage.h"
#include "Raft.h"
#include "Store.h"
#include "TiKVServer.h"
#include "keys.h"
#include "RaftLog.h"

boost::optional<raft_cmdpb::TransferLeaderRequest> get_transfer_leader_cmd(const raft_cmdpb::RaftCmdRequest& msg) {
    if (!msg.has_admin_request()) {
        return boost::optional<raft_cmdpb::TransferLeaderRequest>(boost::none);
    }
    auto req = msg.admin_request();
    if (!req.has_transfer_leader()) {
        return boost::optional<raft_cmdpb::TransferLeaderRequest>(boost::none);
    }
	return req.transfer_leader();
}

boost::optional<raft_cmdpb::ChangePeerRequest> get_change_peer_cmd(const raft_cmdpb::RaftCmdRequest& msg){
	if (!msg.has_admin_request()) {
		return boost::optional<raft_cmdpb::ChangePeerRequest>(boost::none);
	}
	auto req = msg.admin_request();
	if (!req.has_change_peer()) {
		return boost::optional<raft_cmdpb::ChangePeerRequest>(boost::none);
	}
	return req.change_peer();
}

PendingCmdQueue::PendingCmdQueue(){
}

PendingCmdQueue::~PendingCmdQueue(){

}

bool PendingCmdQueue::contains(std::string uuid) const {
	return uuids.count(uuid) != 0;
}

void PendingCmdQueue::remove(boost::optional<PendingCmd> cmd) {
	if (cmd){
		auto it = this->uuids.find(cmd.get().uuid);
		this->uuids.erase(it);
	}
}

boost::optional<PendingCmd> PendingCmdQueue::pop_normal(uint64_t term){
	if (this->normals.size() == 0){
		return boost::optional<PendingCmd>(boost::none);
	}
	PendingCmd cmd = this->normals.back();
	if (cmd.term > term){
		return boost::optional<PendingCmd>(boost::none);
	}else{
		this->normals.pop_back();
		this->remove(cmd);
		return cmd;
	}
}

void PendingCmdQueue::append_normal(PendingCmd& cmd) {
	this->uuids.insert(cmd.uuid);
	this->normals.push_back(cmd);
}

boost::optional<PendingCmd> PendingCmdQueue::take_conf_change(){
	// conf change will not be affected when changing between follower and leader,
	// so there is no need to check term.
	if (this->conf_change){
		PendingCmd cmd = this->conf_change.get();
		this->remove(cmd);
		this->conf_change = boost::optional<PendingCmd>(boost::none);
		return cmd;
	}
	return boost::optional<PendingCmd>(boost::none);
}

void PendingCmdQueue::set_conf_change(PendingCmd cmd) {
	this->uuids.insert(cmd.uuid);
	this->conf_change = cmd;
}

ExecContext::ExecContext(raft_serverpb::RaftApplyState apply_state, 
		raft_cmdpb::RaftCmdRequest& req) : apply_state(apply_state), req(req){
}

ExecContext::~ExecContext(){
}

void ExecContext::save(uint64_t region_id){
	int apply_state_msg_size = this->apply_state.ByteSize();
	char* apply_state_buffer = new char[apply_state_msg_size + 1];
	apply_state_buffer[apply_state_msg_size] = '\0';
	this->apply_state.SerializeToArray(apply_state_buffer, apply_state_msg_size);
	wb.Put(apply_state_key(region_id), std::string(apply_state_buffer, apply_state_msg_size));
	delete []apply_state_buffer;
	LOG_INFO << "ExecContext::save_apply:" << this->apply_state.DebugString();
}

// a helper function to create peer easily.
metapb::Peer new_peer(uint64_t store_id, uint64_t peer_id) {
    auto peer = metapb::Peer();
    peer.set_store_id(store_id);
    peer.set_id(peer_id);
    return peer;
}

metapb::Peer find_peer(metapb::Region region, uint64_t store_id){
	for (int i = 0; i < region.peers_size(); ++i){
		metapb::Peer p = region.peers(i);
		if (p.store_id() == store_id) {
			return p;
		}
	}
	return metapb::Peer();
}

Peer::Peer(rocksdb::DB* db, Store* store_, metapb::Region region): db(db), store(store_){	
	slash::BaseConf* conf = TiKVConfig::getInstance()->getBaseConf();
	PeerStorage* ps = new PeerStorage(db, region);
	this->peer_storage = ps;
	Config config = Config::getInstance();

	auto p = find_peer(region, store_->get_store_id());
	RaftConfig raft_conf;
	raft_conf.id = p.id();
	raft_conf.peers = std::vector<uint64_t>();
	raft_conf.election_tick = config.get_raft_election_timeout_ticks();
	raft_conf.heartbeat_tick = config.get_raft_heartbeat_ticks();
	raft_conf.max_size_per_msg = config.get_raft_max_size_per_msg();
	raft_conf.max_inflight_msgs = config.get_raft_max_inflight_msgs();
	raft_conf.applied = ps->applied_index();
	raft_conf.check_quorum = true;
	raft_conf.pre_vote = true;
	raft_conf.tag = "raft-group-1";

	raft_group = new RawNode(raft_conf, ps);

	for (int i = 0; i < region.peers_size(); ++i){
		const metapb::Peer p = region.peers(i);
		this->peer_cache.insert(std::make_pair(p.id(), p));
	}
	this->peer = new_peer(store_->get_store_id(), p.id());
	this->region_id = region.id();
}

Peer::~Peer(){
}


PeerStorage* Peer::get_store() {
	//LOG_INFO << "addr:" << this->peer_storage;
	return this->peer_storage;
}

Peer* Peer::create(){
	return NULL;
}

// The peer can be created from another node with raft membership changes, and we only
// know the region_id and peer_id when creating this replicated peer, the region info
// will be retrieved later after applying snapshot.
// 有peer_id和region_id就可以创建出Peer来
Peer* Peer::replicate(){
	return NULL;
}

metapb::Region Peer::region() {
	return this->get_store()->get_region();
}

/// Propose a request.
///
/// Return true means the request has been proposed successfully.
bool Peer::propose(PendingCmd cmd, raft_cmdpb::RaftCmdRequest req, raft_cmdpb::RaftCmdResponse& resp){
	if (this->pending_cmds.contains(cmd.uuid)) {
		//let e: Error = box_err!("duplicated uuid {:?}", cmd.uuid);
		resp.mutable_header()->mutable_error()->set_message("duplicated uuid " + cmd.uuid);
		cmd.cb(resp);
		return false;
	}
	LOG_INFO << this->tag << " propose command with uuid " << cmd.uuid;

	bool local_read = this->is_local_read(req);
	if (local_read) {
		//	// for read-only, if we don't care stale read, we can
		//	// execute these commands immediately in leader.
		//	let engine = self.engine.clone();
		//	let mut ctx = ExecContext {
		//snap//: Snapshot::new(engine),
		//	  apply_state: self.get_store().apply_state.clone(),
		//	  wb: WriteBatch::new(),
		//	  req: &req,
		//	};
		//	let (mut resp, _) = self.exec_raft_cmd(&mut ctx).unwrap_or_else(|e| {
		//			error!("{} execute raft command err: {:?}", self.tag, e);
		//			let mut resp = RaftCmdResponse::new();
		//			resp.mut_header().set_error(e.into());

		//			(resp, None)
		//			});
		resp.mutable_header()->set_uuid(cmd.uuid);
		if (this->term() > 0){
			resp.mutable_header()->set_current_term(this->term());
		}
		cmd.cb(resp);
	} 
	else if (get_transfer_leader_cmd(req) ){
	//	let transfer_leader = get_transfer_leader_cmd(&req).unwrap();
	//	let peer = transfer_leader.get_peer();

	//	if self.is_tranfer_leader_allowed(peer) {
	//		self.transfer_leader(peer);
	//	} else {
	//		info!("{} transfer leader message {:?} ignored directly",
	//				self.tag,
	//				req);
	//	}

	//	// transfer leader command doesn't need to replicate log and apply, so we
	//	// return immediately. Note that this command may fail, we can view it just as an advice
	//	return cmd.cb.call_box((make_transfer_leader_response(),));
	//} else if get_change_peer_cmd(&req).is_some() {
	//	if self.raft_group.raft.pending_conf {
	//		return Err(box_err!("there is a pending conf change, try later"));
	//	}
	//	if let Some(cmd) = self.pending_cmds.take_conf_change() {
	//		// if it loses leadership before conf change is replicated, there may be
	//		// a stale pending conf change before next conf change is applied. If it
	//		// becomes leader again with the stale pending conf change, will enter
	//		// this block, so we notify leadership may have changed.
	//		self.notify_not_leader(cmd);
	//	}

	//	if let Err(e) = self.propose_conf_change(req) {
	//		err_resp.mut_header().set_error(e.into());
	//		return cmd.cb.call_box((err_resp,));
	//	}

		this->pending_cmds.set_conf_change(cmd);
	} 
	else if (this->propose_normal(req) == false) {
	//	err_resp.mut_header().set_error(e.into());
	//	return cmd.cb.call_box((err_resp,));
	} else {
		this->pending_cmds.append_normal(cmd);
	}
	return true;
}

void Peer::handle_raft_ready(){
	if (!this->raft_group->has_ready()) {
		return;
	}
	auto ready = this->raft_group->ready();

	if (this->is_leader()){
		this->send(ready.messages);
	}
	this->peer_storage->handle_raft_ready(ready);
	//不是leader
	if (!this->is_leader()) {
		this->send(ready.messages);
	}
	this->handle_raft_commit_entries(ready.committed_entries);
	this->raft_group->advance(ready);
}

void Peer::send(std::vector<eraftpb::Message>& msgs){
	for (size_t i = 0; i < msgs.size(); ++i){
		this->send_raft_message(msgs[i]);
	}
}

void Peer::send_raft_message(eraftpb::Message& msg){
	msgpb::Message send_msg;

	send_msg.set_msg_type(msgpb::MessageType::Raft);
	raft_serverpb::RaftMessage* raft_msg = send_msg.mutable_raft();
	raft_msg->set_region_id(this->region().id());

	metapb::RegionEpoch* region_epoch = raft_msg->mutable_region_epoch();
	region_epoch->CopyFrom(this->region().region_epoch());

	eraftpb::Message* m = raft_msg->mutable_message();
	m->CopyFrom(msg);

	metapb::Peer from_peer = this->get_peer_from_cache(msg.from()).get();
	metapb::Peer to_peer = this->get_peer_from_cache(msg.to()).get();

	uint64_t to_peer_id = to_peer.id();
	uint64_t to_store_id = to_peer.store_id();

	metapb::Peer* from_p = raft_msg->mutable_from_peer();
	metapb::Peer* to_p = raft_msg->mutable_to_peer();
	from_p->CopyFrom(from_peer);
	to_p->CopyFrom(to_peer);	
	this->store->get_server()->runInLoop(boost::bind(&TiKVServer::sendToStore, 
				this->store->get_server(), to_store_id, send_msg));
}

bool Peer::is_leader(){
	return this->raft_group->raft->get_state() == StateRole::Leader;
}

void Peer::step(const eraftpb::Message& m){
	if (this->is_leader() && m.from() != INVALID_ID) {
		this->peer_heartbeats.insert(std::make_pair(m.from(), time(NULL)));
	}
	this->raft_group->step(m);
}

uint64_t Peer::term(){
	return raft_group->raft->get_term();
}

bool Peer::check_epoch(const raft_cmdpb::RaftCmdRequest& req)  {
	auto check_ver = false;
	auto check_conf_ver = false;
	//if req.has_admin_request() {
	//	match req.get_admin_request().get_cmd_type() {
	//		AdminCmdType::CompactLog |
	//			AdminCmdType::InvalidAdmin |
	//			AdminCmdType::ComputeHash |
	//			AdminCmdType::VerifyHash => {}
	//		AdminCmdType::Split => check_ver = true,
	//			AdminCmdType::ChangePeer => check_conf_ver = true,
	//			AdminCmdType::TransferLeader => {
	//				check_ver = true;
	//				check_conf_ver = true;
	//			}
	//	};
	//} else {
	// for get/set/delete, we don't care conf_version.
	check_ver = true;
	//}

	if (!check_ver && !check_conf_ver) {
		return true;
	}

	if (!req.header().has_region_epoch()) {
		return false;
	}

	auto from_epoch = req.header().region_epoch();
	auto latest_region = this->region();
	auto latest_epoch = latest_region.region_epoch();

	// should we use not equal here?
	if ((check_conf_ver && from_epoch.conf_ver() < latest_epoch.conf_ver()) ||
			(check_ver && from_epoch.version() < latest_epoch.version())){
		LOG_WARN << this->tag << " received stale epoch " << from_epoch.DebugString() << ", mime: " << latest_epoch.DebugString();
		return false;
	}
	return true;
}

std::vector<ExecResult> Peer::handle_raft_commit_entries(
		std::vector<eraftpb::Entry>& committed_entries){
	// If we send multiple ConfChange commands, only first one will be proposed correctly,
	// others will be saved as a normal entry with no data, so we must re-propose these
	// commands again.
	std::vector<ExecResult> results;
	size_t committed_count = committed_entries.size();
	for (auto& entry : committed_entries) {
		boost::optional<ExecResult> ret;
		switch (entry.entry_type()) {
			case eraftpb::EntryType::EntryNormal:
				ret = handle_raft_entry_normal(entry);
				break;
			case eraftpb::EntryType::EntryConfChange:
				ret = this->handle_raft_entry_conf_change(entry);
				break;
		}
		if (ret) {
			results.push_back(ret.get());
		}
	}
	if (committed_count > 0)
		LOG_INFO << this->tag << " handle " << committed_count <<" committed entries";
	return results;
}

boost::optional<ExecResult> Peer::handle_raft_entry_normal(eraftpb::Entry& entry) {
	LOG_INFO << "Peer::handle_raft_entry_normal:" << entry.DebugString();
	auto index = entry.index();
	auto term = entry.term();
	auto data = entry.data();

	if (data.size() == 0) {
		// when a peer become leader, it will send an empty entry.
		rocksdb::WriteBatch wb;
		auto state = this->peer_storage->get_apply_state();
		state.set_applied_index(index);

		int apply_state_msg_size = state.ByteSize();
		char* apply_state_buffer = new char[apply_state_msg_size + 1];
		apply_state_buffer[apply_state_msg_size] = '\0';
		state.SerializeToArray(apply_state_buffer, apply_state_msg_size);

		wb.Put(apply_state_key(this->region_id), std::string(apply_state_buffer, apply_state_msg_size));
		delete []apply_state_buffer;

		auto s = this->db->Write(rocksdb::WriteOptions(), &wb);
		assert(s.ok());

		LOG_INFO << "region:[" << this->region_id << "] apply_state:" << state.DebugString();
		this->peer_storage->set_apply_state(state);
		this->peer_storage->set_applied_index_term(term);
		return boost::optional<ExecResult>(boost::none);
	}

	//从rocksdb中获取
	typedef raft_cmdpb::RaftCmdRequest T;
	std::string typeName = T::descriptor()->full_name();
	MessagePtr message;
	message.reset(createMessage(typeName));
	assert(message != NULL);
	if (message->ParseFromArray(data.data(), data.size())) {
		auto cmd = muduo::down_pointer_cast<raft_cmdpb::RaftCmdRequest>(message);
		this->process_raft_cmd(index, term, *cmd.get());
	}else{
		LOG_FATAL << "ParseFromArray Error";
		exit(1);
	}
	return boost::optional<ExecResult>(boost::none);
}

boost::optional<ExecResult> Peer::handle_raft_entry_conf_change(eraftpb::Entry& entry){
	LOG_INFO << "Peer::handle_raft_entry_conf_change: " << entry.DebugString();
	auto index = entry.index();
	auto term = entry.term();

	return boost::optional<ExecResult>(boost::none);
	//let mut conf_change =
	//    try!(protobuf::parse_from_bytes::<eraftpb::ConfChange>(entry.get_data()));
	//let cmd = try!(protobuf::parse_from_bytes::<RaftCmdRequest>(conf_change.get_context()));
	//
	//let res = match self.process_raft_cmd(index, term, cmd) {
	//    a @ Ok(Some(_)) => a,
	//    e => {
	//        error!("{} process raft command at index {} err: {:?}",
	//               self.tag,
	//               index,
	//               e);
	//        // If failed, tell raft that the config change was aborted.
	//        conf_change = eraftpb::ConfChange::new();
	//        Ok(None)
	//    }
	//};
	//self.raft_group.apply_conf_change(conf_change);
	//res
}

bool Peer::is_local_read(raft_cmdpb::RaftCmdRequest& req) {
	if ((req.has_header() && req.header().read_quorum()) ||
			!this->raft_group->raft->in_lease() || req.requests().size() == 0) {
		return false;
	}
	// If applied index's term is differ from current raft's term, leader transfer
	// must happened, if read locally, we may read old value.
	if (this->peer_storage->get_applied_index_term() != this->raft_group->raft->get_term()) {
		return false;
	}

	for(auto& cmd_req: req.requests()){
		if (cmd_req.cmd_type() != raft_cmdpb::CmdType::Snap && 
				cmd_req.cmd_type() != raft_cmdpb::CmdType::Get){
			return false;
		}
	}
	return true;
}

uint64_t Peer::next_proposal_index(){
	return this->raft_group->raft->raft_log->last_index() + 1;
}

bool Peer::propose_normal(raft_cmdpb::RaftCmdRequest cmd){
	// TODO: validate request for unexpected changes.
	int data_size = cmd.ByteSize();
	char* data_buffer = new char[data_size + 1];
	data_buffer[data_size] = '\0';
	cmd.SerializeToArray(data_buffer, data_size);
	//if data.len() as u64 > self.raft_entry_max_size {
	//    error!("entry is too large, entry size {}", data.len());
	//    return Err(Error::RaftEntryTooLarge(self.region_id, data.len() as u64));
	//}

	uint64_t propose_index = this->next_proposal_index();
	this->raft_group->propose(std::string(data_buffer, data_size));
	if (this->next_proposal_index() == propose_index) {
		// The message is dropped silently, this usually due to leader absence
		// or transferring leader. Both cases can be considered as NotLeader error.
		return false;
		//return Err(Error::NotLeader(self.region_id, None));
	}
	return true;
}

uint64_t Peer::leader_id()  {
	return this->raft_group->raft->get_leader_id();
}

void Peer::notify_not_leader(const PendingCmd& cmd) {
	auto leader = this->get_peer_from_cache(this->leader_id()).get();
	auto resp = raft_cmdpb::RaftCmdResponse();
	resp.mutable_header()->mutable_error()->set_message("not leader");
	resp.mutable_header()->set_current_term(this->term());
	resp.mutable_header()->set_uuid(cmd.uuid);
	LOG_WARN << this->tag << " command " << cmd.uuid << " is stale, skip";
	cmd.cb(resp);
}

boost::optional<ResponseCallback> Peer::find_cb(std::string uuid, uint64_t term, 
		const raft_cmdpb::RaftCmdRequest& cmd) {
	if (get_change_peer_cmd(cmd)) {
		auto cur_cmd = this->pending_cmds.take_conf_change();
		if (cur_cmd.get().uuid == uuid) {
			return cur_cmd.get().cb;
		} else {
			this->notify_not_leader(cur_cmd.get());
		}
	}
	while (true){
		auto head = this->pending_cmds.pop_normal(term);	
		if (head){
			if (head.get().uuid == uuid) {
				return head.get().cb;
			}
			// because of the lack of original RaftCmdRequest, we skip calling
			// coprocessor here.
			// TODO: call coprocessor with uuid instead.
			this->notify_not_leader(head.get());
		}else{
			break;
		}
	}
	return boost::optional<ResponseCallback>(boost::none);
}

void Peer::process_raft_cmd(uint64_t index, uint64_t term,
		raft_cmdpb::RaftCmdRequest& cmd){
	if (index == 0) {
		return;
		//return Err(box_err!("processing raft command needs a none zero index"));
	}

	std::string uuid = get_uuid_from_req(cmd);
	auto cb = this->find_cb(uuid, term, cmd);

	raft_cmdpb::RaftCmdResponse resp = this->apply_raft_cmd(index, term, cmd);
	//		error!("{} apply raft command err {:?}", self.tag, e);
	//		let mut resp = RaftCmdResponse::new();
	//		resp.mut_header().set_error(e.into());
	//		(resp, None)
	//		});

	LOG_INFO << this->tag << " applied command with uuid "<< uuid << " at log index " << index;
	if (!cb){
		return;
	} 
	// TODO: if we have exec_result, maybe we should return this callback too. Outer
	// store will call it after handing exec result.
	// Bind uuid here.
	resp.mutable_header()->set_uuid(uuid);
	resp.mutable_header()->set_current_term(this->term());
	cb.get()(resp);
}

raft_cmdpb::RaftCmdResponse Peer::apply_raft_cmd(
		uint64_t index,
		uint64_t term,
		raft_cmdpb::RaftCmdRequest& req){
	uint64_t last_applied_index = this->peer_storage->applied_index();
	if (last_applied_index >= index) {
		LOG_FATAL << "applied index moved backwards, " << last_applied_index << " >=  << index";	
		raft_cmdpb::RaftCmdResponse resp;
		resp.mutable_header()->mutable_error()->set_message("applied index moved backwards");
		resp.mutable_header()->set_current_term(this->term());
		return resp;
	}

	ExecContext ctx(this->peer_storage->get_apply_state(), req);
	auto resp = this->exec_raft_cmd(ctx);

	ctx.apply_state.set_applied_index(index);
	ctx.save(this->region_id);

	auto s = this->db->Write(rocksdb::WriteOptions(), &ctx.wb);
	assert(s.ok());
	return resp;
}

raft_cmdpb::RaftCmdResponse Peer::exec_raft_cmd(ExecContext& ctx) {
	if (ctx.req.has_admin_request()) {
		//return this->exec_admin_cmd(ctx);
	} else {
		// Now we don't care write command outer, so use None.
		return this->exec_write_cmd(ctx);
	}
	return raft_cmdpb::RaftCmdResponse();
}

raft_cmdpb::RaftCmdResponse Peer::exec_write_cmd(ExecContext& ctx){
	auto requests = ctx.req.requests();
	auto response = raft_cmdpb::RaftCmdResponse();
	for (auto& req : requests) {
		auto cmd_type = req.cmd_type();
		raft_cmdpb::Response resp;
		switch (cmd_type) {
			case raft_cmdpb::CmdType::Get:
				//this->do_get(ctx, req);
				break;	
			case raft_cmdpb::CmdType::Put:
				resp = this->do_put(ctx, req);
				break;
			case raft_cmdpb::CmdType::Delete:
				//this->do_delete(ctx, req);
				break;
			case raft_cmdpb::CmdType::Snap:
				//this->do_delete(ctx, req);
				break;
			case raft_cmdpb::CmdType::Invalid:
				break;
		}
		resp.set_cmd_type(cmd_type);
		response.add_responses()->CopyFrom(resp);
	}
	return response;
}

raft_cmdpb::Response Peer::do_put(ExecContext& ctx, raft_cmdpb::Request& req) {
	auto key = req.put().key();
	auto value = req.put().value();
	LOG_INFO << "exec cmd do_put, key:" << key << " value:" << value;
	return raft_cmdpb::Response();
}

