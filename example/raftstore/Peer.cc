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

Peer::Peer(rocksdb::DB* db, Store* store_, metapb::Region region):store(store_){	
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
bool Peer::propose(PendingCmd cmd, raft_cmdpb::RaftCmdRequest req){
	//if self.pending_cmds.contains(&cmd.uuid) {
	//	let e: Error = box_err!("duplicated uuid {:?}", cmd.uuid);
	//	err_resp.mut_header().set_error(e.into());
	//	return cmd.cb.call_box((err_resp,));
	//}

	//debug!("{} propose command with uuid {:?}", self.tag, cmd.uuid);

	//let local_read = self.is_local_read(&req);
	//if local_read {
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

	//	resp.mut_header().set_uuid(cmd.uuid.as_bytes().to_vec());
	//	if self.term() > 0{
	//		resp.mut_header().set_current_term(self.term());
	//	}
	//	return cmd.cb.call_box((resp,));
	//} 
	//else if get_transfer_leader_cmd(&req).is_some() {
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

	//	self.pending_cmds.set_conf_change(cmd);
	//} 
	//else if let Err(e) = self.propose_normal(req) {
	//	err_resp.mut_header().set_error(e.into());
	//	return cmd.cb.call_box((err_resp,));
	//} else {
	//	self.pending_cmds.append_normal(cmd);
	//}

	//Ok(())
		return true;
}

void Peer::handle_raft_ready(){
	if (!this->raft_group->has_ready()) {
		return;
	}
	LOG_INFO << this->tag << " handle raft ready";
	auto ready = this->raft_group->ready();

	this->peer_storage->handle_raft_ready(ready);

	this->send(ready.messages);
	this->raft_group->advance(ready);
}

void Peer::send(std::vector<eraftpb::Message>& msgs){
	for (size_t i = 0; i < msgs.size(); ++i){
		LOG_INFO << "Peer::send " << msgs[i].DebugString();
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

	metapb::Peer from_peer = this->get_peer_from_cache(msg.from());
	metapb::Peer to_peer = this->get_peer_from_cache(msg.to());

	uint64_t to_peer_id = to_peer.id();
	uint64_t to_store_id = to_peer.store_id();

	//msg_type = msg.get_msg_type();
	//debug!("{} send raft msg {:?}[size: {}] from {} to {}",
	//		self.tag,
	//		msg_type,
	//		msg.compute_size(),
	//		from_peer.get_id(),
	//		to_peer_id);
	metapb::Peer* from_p = raft_msg->mutable_from_peer();
	metapb::Peer* to_p = raft_msg->mutable_to_peer();
	from_p->CopyFrom(from_peer);
	to_p->CopyFrom(to_peer);	
	LOG_INFO << "Peer::send_raft_message: " << send_msg.DebugString();
	this->store->get_server()->runInLoop(boost::bind(&TiKVServer::sendToStore, this->store->get_server(), to_store_id, send_msg));
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
