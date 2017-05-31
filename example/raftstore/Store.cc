#include "Store.h"
#include "TiKVServer.h"

#include "Peer.h"
#include "TiKVConf.h"

#include <sstream>
#include <boost/bind.hpp>

Store::Store(StoreRouter* router_, TiKVServer* server_, uint64_t store_id, rocksdb::DB* db_): 
	store_id_(store_id), server(server_), router(router_){
	region_peers.clear();
	pending_raft_groups.clear();
	pending_regions.clear();
	peer_cache.clear();
	this->db = db_;
}

Store::~Store(){
	delete thread_;
}

void Store::schedule_task(){
	std::map<uint64_t, Peer*>::iterator it = region_peers.begin();
	while (it != region_peers.end()){
		it->second->raft_group->tick();
		it->second->handle_raft_ready();
		++it;
	}
}

void Store::heartbeat_pd(Peer* peer) {
	metapb::Region region =  peer->region();
	region.set_term(peer->term());
	std::vector<pdpb::PeerStats> down_peers;

	this->router->run_in_work_Loop(boost::bind(&StoreRouter::handle_heartbeat, 
				this->router, region, peer->get_peer(), down_peers)); 
}

void Store::on_pd_heartbeat_tick(){
	//for peer in self.region_peers.values_mut() {
	//	peer.check_peers();
	//}
	for (auto &kv : this->region_peers) {
		if (kv.second->is_leader()){
			this->heartbeat_pd(kv.second);
		}
	}
}

void Store::thread_init_func(muduo::net::EventLoop* loop_){
}

void Store::add_peer(uint64_t region_id, Peer* p){
	this->region_peers.insert(std::make_pair(region_id, p));
}

bool Store::init(){
	thread_ = new muduo::net::EventLoopThread(boost::bind(&Store::thread_init_func, this, _1));
	loop_ = thread_->startLoop();

	loop_->runEvery(RAFT_BASE_TICK_INTERVAL * 0.001, boost::bind(&Store::schedule_task, this));
	loop_->runEvery(PD_HEARTBEAT_TICK_INTERVAL* 0.001, boost::bind(&Store::on_pd_heartbeat_tick, this));
	return true;
}

// return false means the message is invalid, and can be ignored.
bool Store::is_raft_msg_valid(const raft_serverpb::RaftMessage& msg) {
	auto msg_region_id = msg.region_id();
	auto from = msg.from_peer();
	auto to = msg.to_peer();

	if (to.store_id() != this->store_id_) {
		LOG_WARN << "[region " << msg_region_id << "] store not match, to store id " 
			<< to.store_id() << ", mine " << this->store_id_ << ", ignore it";
		return false;
	}

	if (!msg.has_region_epoch()) {
		LOG_ERROR << "[region " << msg_region_id << "] missing epoch in raft message, ignore it";
		return false;
	}
	return true;
}

Result<bool, Error> Store::check_target_peer_valid(uint64_t region_id, metapb::Peer target) {
	// we may encounter a message with larger peer id, which means
	// current peer is stale, then we should remove current peer
	bool has_peer = false;
	//let mut stale_peer = None;
	if (this->region_peers.count(region_id) > 0) {
		has_peer = true;
	//	let target_peer_id = target.get_id();
	//	if p.peer_id() < target_peer_id {
	//		if p.is_applying_snapshot() && !p.mut_store().cancel_applying_snap() {
	//			warn!("[region {}] Stale peer {} is applying snapshot, will destroy next \
	//					time.",
	//					region_id,
	//					p.peer_id());
	//			return Ok(false);
	//		}
	//		stale_peer = Some(p.peer.clone());
	//	} else if p.peer_id() > target_peer_id {
	//		warn!("target peer id {} is less than {}, msg maybe stale.",
	//				target_peer_id,
	//				p.peer_id());
	//		return Ok(false);
	//	}
	//}
	//if let Some(p) = stale_peer {
	//	info!("[region {}] destroying stale peer {:?}", region_id, p);
	//	self.destroy_peer(region_id, p);
	//	has_peer = false;
	}

	if (!has_peer) {
		auto peer = Peer::replicate(this->db, this, region_id, target.id());
		// We don't have start_key of the region, so there is no need to insert into
		// region_ranges
		this->region_peers.insert(std::make_pair(region_id, peer));
	}
	return Ok(true);
}

void Store::insert_peer_cache(const metapb::Peer& peer) {
	this->peer_cache.insert(std::make_pair(peer.id(), peer));
}

boost::optional<metapb::Peer> Store::get_peer_from_cache(uint64_t peer_id){
	auto it = this->peer_cache.find(peer_id);
	if (it != this->peer_cache.end()){
		return it->second;
	}
	return boost::optional<metapb::Peer>(boost::none);
}
void Store::on_raft_message(const raft_serverpb::RaftMessage raft){
	//to_peer字段中的store_id是否是当前store_id
	if (!this->is_raft_msg_valid(raft)) {
		LOG_INFO << "on_raft_message is_raft_msg_valid, false, msg:" << raft.DebugString();
		return;
	}
	//目标Peer是否存在，如果不存在，可能是添加分片的请求，就创建出来Peer来
	auto check_target_resp = this->check_target_peer_valid(raft.region_id(), raft.to_peer());
	if (check_target_resp.isErr() || (check_target_resp.isOk() && !check_target_resp.unwrap())) {
	    return;
	}

	this->insert_peer_cache(raft.from_peer());
	this->insert_peer_cache(raft.to_peer());

	uint64_t region_id = raft.region_id();
	std::map<uint64_t, Peer* >::iterator it = region_peers.find(region_id);
	if (it != region_peers.end()){
		Peer* peer = it->second;
		peer->step(raft.message());
	}else{
		LOG_WARN << "Peer is null";
	}
}

void Store::on_command_message(const raft_cmdpb::RaftCmdRequest& cmd, ResponseCallback callback){
	this->propose_raft_command(cmd, callback);
}

bool Store::validate_store_id(const raft_cmdpb::RaftCmdRequest& msg) {
	uint64_t store_id = msg.header().peer().store_id();
	if (store_id != this->get_store_id()) {
		return false;
	}
	return true;
}

bool Store::validate_region(const raft_cmdpb::RaftCmdRequest& msg) {
	return true;
	//auto region_id = msg.header().region_id();
	//auto peer_id = msg.header().peer().id();

	//Peer* p = NULL;
	//auto it = this->region_peers.find(region_id);
	//if (it == this->region_peers.end()){
	//	return false;
	//}
	//p = it->second;
	//if (!p->is_leader()) {
	//	return false;
	//}
	//if (p->peer_id() != peer_id) {
	//	return false;
	//}

	//auto header = msg.header();
	//// If header's term is 2 verions behind current term, leadership may have been changed away.
	//if (header.term() > 0 && p->term() > header.term() + 1) {
	//	return false;
	//}

	//return p->check_epoch(msg);
}

void Store::propose_raft_command(const raft_cmdpb::RaftCmdRequest& msg, ResponseCallback callback) {
	LOG_INFO << "store propose_raft_command:" << msg.DebugString();
	std::string uuid = get_uuid_from_req(msg);
	auto resp = raft_cmdpb::RaftCmdResponse();
	if (uuid.size() == 0){
		LOG_WARN << "uuid is null! RaftCmdRequest:" << msg.DebugString();
		resp.mutable_header()->mutable_error()->set_message("missing request uuid");
		callback(resp);
	}else{
		resp.mutable_header()->set_uuid(uuid);
	}

	if (!this->validate_store_id(msg)) {
		resp.mutable_header()->mutable_error()->set_message("not validate store id");
		LOG_WARN << "validate_store_id, is not validate store id";
		callback(resp);
		return;
	}
	LOG_INFO << "validate_store_id, is validate store id";

	if (!this->validate_region(msg)) {
		LOG_WARN << "validate_region, is not validate region id, ";
		resp.mutable_header()->mutable_error()->set_message("not validate region id");
		callback(resp);
		return;
	}
	LOG_INFO << "validate_region, is validate region id";
	// Note:
	// The peer that is being checked is a leader. It might step down to be a follower later. It
	// doesn't matter whether the peer is a leader or not. If it's not a leader, the proposing
	// command log entry can't be committed.
	auto region_id = msg.header().region_id();
	auto peer = this->region_peers[region_id];
	auto term = peer->term();
	struct PendingCmd pending_cmd = {uuid, term, callback };
	peer->propose(pending_cmd, msg, resp);
	//self.pending_raft_groups.insert(region_id);
}

