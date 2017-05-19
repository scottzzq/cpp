#include "Store.h"
#include "TiKVServer.h"

#include "Peer.h"
#include "TiKVConf.h"

#include <sstream>
#include <boost/bind.hpp>

std::string get_uuid_from_req(const raft_cmdpb::RaftCmdRequest& msg){
	return msg.header().uuid();
}

Store::Store(TiKVServer* server_, uint64_t store_id): store_id_(store_id), server(server_){
	region_peers.clear();
	pending_raft_groups.clear();
	pending_regions.clear();
	peer_cache.clear();
}

Store::~Store(){
	delete thread_;
}

void Store::scheduleTask(){
	LOG_INFO << "Store::scheduleTask";
	std::map<uint64_t, Peer*>::iterator it = region_peers.begin();
	while (it != region_peers.end()){
		it->second->raft_group->tick();
		it->second->handle_raft_ready();
		++it;
	}
}

void Store::threadInitFunc(muduo::net::EventLoop* loop_){
	loop_->runEvery(1, boost::bind(&Store::scheduleTask, this));
}

void Store::add_peer(uint64_t region_id, Peer* p){
	region_peers.insert(std::make_pair(region_id, p));
}

bool Store::init(){
	thread_ = new muduo::net::EventLoopThread(boost::bind(&Store::threadInitFunc, this, _1));
	loop_ = thread_->startLoop();
	return true;
}

void Store::on_raft_message(const raft_serverpb::RaftMessage raft){
	uint64_t region_id = raft.region_id();
	std::map<uint64_t, Peer* >::iterator it = region_peers.find(region_id);
	if (it != region_peers.end()){
		Peer* peer = it->second;
		peer->step(raft.message());
	}
}

void Store::on_command_message(const raft_cmdpb::RaftCmdRequest& cmd, ResponseCallback callback){

}

bool Store::validate_store_id(const raft_cmdpb::RaftCmdRequest& msg) {
	uint64_t store_id = msg.header().peer().store_id();
	if (store_id != this->get_store_id()) {
		return false;
	}
	return true;
}

bool Store::validate_region(const raft_cmdpb::RaftCmdRequest& msg) {
	auto region_id = msg.header().region_id();
	auto peer_id = msg.header().peer().id();

	Peer* p = NULL;
	auto it = this->region_peers.find(region_id);
	if (it == this->region_peers.end()){
		return false;
	}
	p = it->second;
	if (!p->is_leader()) {
		return false;
	}
	if (p->peer_id() != peer_id) {
		return false;
	}

	auto header = msg.header();
	// If header's term is 2 verions behind current term, leadership may have been changed away.
	if (header.term() > 0 && p->term() > header.term() + 1) {
		return false;
	}

	return p->check_epoch(msg);
}

void Store::propose_raft_command(const raft_cmdpb::RaftCmdRequest& msg) {
	LOG_INFO << "store propose_raft_command";
	std::string uuid = get_uuid_from_req(msg);
	if (uuid.size() == 0){
	}else{
	}
	if (!this->validate_store_id(msg)) {
		LOG_WARN << "validate_store_id, is not validate store id";
		return;
	}
	LOG_INFO << "validate_store_id, is validate store id";

	if (!this->validate_region(msg)) {
		LOG_WARN << "validate_region, is not validate region id, ";
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
	struct PendingCmd pending_cmd = {uuid, term };
	peer->propose(pending_cmd, msg);
	//self.pending_raft_groups.insert(region_id);
}

