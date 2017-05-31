#include "StoreRouter.h"
#include "TiKVServer.h"

#include <sstream>
#include <boost/bind.hpp>
#include "Peer.h"
#include "keys.h"
#include "RpcClient.h"
#include <uuid/uuid.h>

const uint64_t INIT_EPOCH_VER = 1;
const uint64_t INIT_EPOCH_CONF_VER = 1;

const size_t MAX_CHECK_CLUSTER_BOOTSTRAPPED_RETRY_COUNT = 60;
const int CHECK_CLUSTER_BOOTSTRAPPED_RETRY_SECONDS = 3;

boost::optional<metapb::Peer> find_peer(metapb::Region region, uint64_t store_id){
	for (int i = 0; i < region.peers_size(); ++i){
		metapb::Peer p = region.peers(i);
		if (p.store_id() == store_id) {
			return p;
		}
	}
	return boost::optional<metapb::Peer>(boost::none);
}

raft_cmdpb::AdminRequest new_change_peer_request(eraftpb::ConfChangeType change_type, const metapb::Peer& peer){
	raft_cmdpb::AdminRequest req;
	req.set_cmd_type(raft_cmdpb::AdminCmdType::ChangePeer);
	req.mutable_change_peer()->set_change_type(change_type);
	req.mutable_change_peer()->mutable_peer()->CopyFrom(peer);
	return req;
}

raft_cmdpb::AdminRequest new_transfer_leader_request(const metapb::Peer& peer){
    raft_cmdpb::AdminRequest req;
    req.set_cmd_type(raft_cmdpb::AdminCmdType::TransferLeader);
	req.mutable_change_peer()->mutable_peer()->CopyFrom(peer);
    return req;
}

Result<bool, Error> StoreRouter::check_cluster_bootstrapped(){
	for (size_t i = 0; i < MAX_CHECK_CLUSTER_BOOTSTRAPPED_RETRY_COUNT; ++i) {
		auto resp = this->pd_client->is_cluster_bootstrapped();
		if (resp.isOk()){
			return Ok(resp.unwrap());
		}else{
			LOG_WARN << "check cluster bootstrapped failed: {:?}";
		}
		sleep(CHECK_CLUSTER_BOOTSTRAPPED_RETRY_SECONDS);
	}
	LOG_WARN << "check cluster bootstrapped failed";
	Error e = {"check cluster bootstrapped failed"};
	return Err(e);
}

Result<void, Error> write_region(rocksdb::DB* db, metapb::Region& region) {
	auto state = raft_serverpb::RegionLocalState();
	state.mutable_region()->CopyFrom(region);

	int msg_size = state.ByteSize();
	char* buffer = new char[msg_size];
	state.SerializeToArray(buffer, msg_size);

	rocksdb::WriteBatch wb;
	wb.Put(region_state_key(region.id()), std::string(buffer, msg_size));

	write_initial_state(db, wb, region.id());
	auto s = db->Write(rocksdb::WriteOptions(), &wb);
	assert(s.ok());

	//LOG_INFO << "write_region:" << region.DebugString() << 
	//	" key_len:" << region_state_key(region.id()) << " value_len:" << std::string(buffer, msg_size);
	delete []buffer;
	return Ok();
}

StoreRouter::StoreRouter(TiKVServer* server, int store_num): server_(server),store_num_(store_num){
	stores_.clear();
	rocksdb::Options options;
	options.create_if_missing = true;
	rocksdb::Status status = rocksdb::DB::Open(options, "./db", &db);
	assert(status.ok());

	slash::BaseConf* conf = TiKVConfig::getInstance()->getBaseConf();
	std::string address;
	conf->GetConfStr("address", &address);
	this->store.set_address(address);

	std::string pd_endpoints;
	conf->GetConfStr("pd_endpoints", &pd_endpoints);
	int pd_port;
	conf->GetConfInt("pd_port", &pd_port);

	int temp_cluster_id;
	conf->GetConfInt("cluster_id", &temp_cluster_id);
	this->cluster_id = temp_cluster_id;
	pd_client = new RpcClient(pd_endpoints, pd_port, this->cluster_id);
	
	work_thread = new muduo::net::EventLoopThread(boost::bind(&StoreRouter::work_thread_init_func, this));
	work_thread_loop = work_thread->startLoop();

	work_thread_loop->runEvery(PD_STORE_HEARTBEAT_TICK_INTERVAL * 0.001, 
			boost::bind(&StoreRouter::on_pd_store_heartbeat_tick, this));
}

StoreRouter::~StoreRouter(){
	for(int i = 0; i < store_num_; ++i){
		delete stores_[i];
	}
	stores_.clear();
}

void StoreRouter::work_thread_init_func(){
	LOG_INFO << "work_thread_init_func";
}

void StoreRouter::handle_store_heartbeat(pdpb::StoreStats stats) {
	auto resp = this->pd_client->store_heartbeat(stats);
	if (!resp.isOk()){
		LOG_WARN << "store heartbeat failed {:?}" << stats.DebugString();
	}else{
		LOG_INFO << "store heartbeat success: " << stats.DebugString();
	}
}

void StoreRouter::on_pd_store_heartbeat_tick(){
	pdpb::StoreStats stats;
	stats.set_capacity(1024 * 1024 * 100);
	stats.set_store_id(this->store.id());
	stats.set_available(1024 * 1024 * 10);
	uint32_t count = 0;
	for (auto& store: stores_){
		count += store->region_count();
	}
	stats.set_region_count(count);
	work_thread_loop->runInLoop(boost::bind(&StoreRouter::handle_store_heartbeat, this, stats));
}

void StoreRouter::handle_heartbeat(metapb::Region region, metapb::Peer peer,
		std::vector<pdpb::PeerStats> down_peers) {
	LOG_INFO << "region handle_heartbeat:" << region.DebugString(); 
	// Now we use put region protocol for heartbeat.
	auto region_resp = this->pd_client->region_heartbeat(region, peer, down_peers);
	if (region_resp.isOk()){
		if (region_resp.unwrap().has_change_peer()) {
			auto change_peer = region_resp.unwrap().change_peer();
			LOG_INFO << "[region " << region.id() << "] try to change peer " << change_peer.change_type() << " " 
				<< change_peer.peer().DebugString() <<" for region " << region.DebugString();
			auto req = new_change_peer_request(change_peer.change_type(), change_peer.peer());
			this->send_admin_request(region, peer, req);

		} else if (region_resp.unwrap().has_transfer_leader()) {
			auto transfer_leader = region_resp.unwrap().transfer_leader();
			LOG_INFO << "[region " << region.id() <<"] try to transfer leader from " 
				<< peer.DebugString() << " to " << transfer_leader.peer().DebugString();
			auto req = new_transfer_leader_request(transfer_leader.peer());
			this->send_admin_request(region, peer, req);
		}
	}else{
		LOG_WARN << "[region " << region.id() << "] failed to send heartbeat: {:?}";
	}
}
// check store, return store id for the engine.
// If the store is not bootstrapped, use INVALID_ID.
Result<uint64_t, Error> StoreRouter::check_store(){
	std::string value;
	rocksdb::Status s = db->Get(rocksdb::ReadOptions(), store_ident_key(), &value);
	if (value.size() == 0){
		return Ok(INVALID_ID);
	}
	typedef raft_serverpb::StoreIdent T;
	std::string typeName = T::descriptor()->full_name();
	MessagePtr message;
	message.reset(createMessage(typeName));
	if (message) {
		const char* data = value.data();
		int32_t dataLen = value.size();
		if (message->ParseFromArray(data, dataLen)) {
			auto ident = muduo::down_pointer_cast<raft_serverpb::StoreIdent>(message);
			LOG_INFO << "check_store local store ident: " << ident->DebugString();
			if (ident->cluster_id() != this->cluster_id) {
				LOG_FATAL << "cluster ID mismatch: local_id " << ident->cluster_id() <<" remote_id " << this->cluster_id;
				exit(1);
			}
			uint64_t store_id = ident->store_id();
			if (store_id == INVALID_ID) {
				Error e = {"invalid store ident {:?}"};
				return Err(e);
			}
			return Ok(store_id);
		}else{
			LOG_FATAL << "ParseFromArray Error";
			exit(1);
		}
	}
	Error e = {"ParseFromArray"};
	return Err(e);
}

Result<uint64_t, Error> StoreRouter::alloc_id() {
	return this->pd_client->alloc_id();
}

Result<uint64_t, Error> StoreRouter::bootstrap_store() {
	auto alloc_store_id = this->alloc_id();
	if (!alloc_store_id.isOk()){
		Error e = {"pd alloc id failed"};
		return Err(e);
	}

	LOG_INFO << "alloc store id "<< alloc_store_id.unwrap();
	uint32_t count = 0;
	rocksdb::Iterator* it = db->NewIterator(rocksdb::ReadOptions());
	it->Seek(MIN_KEY);
	for (; it->Valid(); it->Next()) {
		if (it->key().ToString() > MAX_KEY){
			break;
		}
		count += 1;
	}

	if (count > 0) {
		LOG_WARN << "store is not empty and has already had data.";
		Error e = {"store is not empty and has already had data."};
		return Err(e);
	}

	raft_serverpb::StoreIdent ident;
	ident.set_cluster_id(this->cluster_id);
	ident.set_store_id(alloc_store_id.unwrap());

	rocksdb::WriteBatch wb;
	int msg_size = ident.ByteSize();
	char* buffer = new char[msg_size];
	ident.SerializeToArray(buffer, msg_size);

	wb.Put(store_ident_key(), std::string(buffer, msg_size));
	delete []buffer;
	auto s = this->db->Write(rocksdb::WriteOptions(), &wb);
	assert(s.ok());

	LOG_INFO << "bootstrap_store StoreIdent:" << ident.DebugString() << "key_len:" << store_ident_key().size() << " value_len:" << msg_size;
	return Ok(alloc_store_id.unwrap());
}

// Bootstrap first region.
Result<metapb::Region, Error> bootstrap_region(
		rocksdb::DB* db,
		uint64_t store_id,
		uint64_t region_id,
		uint64_t peer_id){
	LOG_INFO << "bootstrap_region, store_id:[" << store_id << "], region_id:[" << region_id << "], peer_id:[" << peer_id << "]";

	metapb::Region region;
	region.set_id(region_id);
	region.set_start_key("");
	region.set_end_key("");
	region.mutable_region_epoch()->set_version(INIT_EPOCH_VER);
	region.mutable_region_epoch()->set_conf_ver(INIT_EPOCH_CONF_VER);

	metapb::Peer* peer = region.add_peers();
	peer->set_store_id(store_id);
	peer->set_id(peer_id);

	auto write_res = write_region(db, region);
	if (write_res.isOk()){
		return Ok(region);
	}
	return Err(write_res.unwrapErr());
}

Result<metapb::Region, Error> StoreRouter::bootstrap_first_region(uint64_t store_id) {
	auto alloc_region_id = this->alloc_id();
	if (!alloc_region_id.isOk()){
		Error e = {"pd alloc id failed"};
		return Err(e);
	}
	LOG_INFO << "alloc first region id " << alloc_region_id.unwrap() << 
		" for cluster " << this->cluster_id << ", store " << store_id;

	auto alloc_peer_id = this->alloc_id();
	if (!alloc_peer_id.isOk()){
		Error e = {"pd alloc pper id failed"};
		return Err(e);
	}
	LOG_INFO << "alloc first peer id " << alloc_peer_id.unwrap() << 
		" for first region " << alloc_region_id.unwrap();

	auto boostrap_res = bootstrap_region(this->db, store_id, alloc_region_id.unwrap(), alloc_peer_id.unwrap());
	if (boostrap_res.isOk()){
		return Ok(boostrap_res.unwrap());
	}
	Error e = {"bootstrap_region error"};
	return Err(e); 
}

Result<void, Error> StoreRouter::bootstrap_cluster(metapb::Region region){
	uint64_t region_id = region.id();
	auto boot_resp = this->pd_client->bootstrap_cluster(this->store, region);
	if (!boot_resp.isOk()){
		return Err(boot_resp.unwrapErr());
	}
	LOG_INFO << "bootstrap cluster " << this->cluster_id <<" ok";
	return Ok();
}

bool StoreRouter::init(){
	auto bootstrapped = this->check_cluster_bootstrapped();
	if (!bootstrapped.isOk()){
		LOG_INFO << "StoreRouter::init check_cluster_bootstrapped false";
		return false;
	}

	auto store_id_res = this->check_store();
	if (!store_id_res.isOk()){
		LOG_INFO << "StoreRouter::init check_store false";
		return false;
	}
	uint64_t store_id = store_id_res.unwrap();

	if (store_id == INVALID_ID) {
		//没有初始化则需要初始化store
		auto new_store_id = this->bootstrap_store();
		if (new_store_id.isOk()){
			store_id = new_store_id.unwrap();
			this->store.set_id(store_id);
			LOG_INFO << "init store_id " << new_store_id.unwrap();
		}else{
			LOG_FATAL << "bootstrap_store error";
			return false;
		}
	} else {
		//store已经被初始化，但是集群还没有初始化
		if (!bootstrapped.unwrap()) {
			// We have saved data before, and the cluster must be bootstrapped.
			LOG_FATAL << "store "<< store_id << " is not empty, but cluster " 
				<< this->cluster_id << "is not bootstrapped";
			return false;
		}else{
			LOG_INFO << "start store store_id:" << store_id;
		}
	}	

	if (!bootstrapped.unwrap()) {
		auto region = this->bootstrap_first_region(store_id);
		if (!region.isOk()){
			return false;
		}
		this->bootstrap_cluster(region.unwrap());
	}
	this->store.set_id(store_id);
	auto put_resp = this->pd_client->put_store(this->store);
	if (put_resp.isOk()){
		LOG_INFO << "put_store success!";
	}else{
		LOG_FATAL << "put_store failed!";
		return false;
	}

	for (int i = 0; i < store_num_; ++i){
		std::stringstream ss;
		ss << i;
		std::string thread_name = "Store-";
		ss >> thread_name;
		Store* store = new Store(this, server_, this->store.id(), this->db);
		stores_.push_back(store);
	}

	// Scan region meta to get saved regions.
	std::string start_key = REGION_META_MIN_KEY;
	std::string end_key = REGION_META_MAX_KEY;

	rocksdb::Iterator* it = db->NewIterator(rocksdb::ReadOptions());
	it->Seek(start_key);
	int count = 0;
	LOG_INFO << "Start Scan rocksdb REGION_META_MIN_KEY";

	std::vector<metapb::Region> regions;
	typedef raft_serverpb::RegionLocalState T;
	std::string typeName = T::descriptor()->full_name();
	//从rocksdb中扫描出每个region的配置信息
	//有多少个region，就有多少条key,value中包含了当前region 包含哪些节点信息
	for (;it->Valid(); it->Next()) {
		//LOG_INFO << "Scan key_len:" << std::string(it->key().data(), it->key().size()) << " value_len:" << std::string(it->value().data(), it->value().size());
		if (it->key().ToString() > end_key){
			break;
		}
		std::tuple<uint64_t, char> region_tuple = 
			decode_region_meta_key(std::string(it->key().data(), it->key().size()));
		assert(std::get<1>(region_tuple) == REGION_STATE_SUFFIX);
		LOG_INFO << "decode region_id:" << std::get<0>(region_tuple); 
		MessagePtr message;
		message.reset(createMessage(typeName));
		if (message) {
			const char* data = it->value().data();
			int32_t dataLen = it->value().size();
			if (message->ParseFromArray(data, dataLen)) {
				count += 1;
				auto msg = muduo::down_pointer_cast<raft_serverpb::RegionLocalState>(message);
				LOG_INFO << msg->DebugString();
				regions.push_back(msg->region());
			}else{
				LOG_WARN << "ParseFromArray Error";
			}
		}
	}
	assert(it->status().ok()); 
	delete it;
	LOG_INFO << "End Scan rocksdb REGION_META_MIN_KEY, count:[" << count << "]";

	//根据扫描出来的meta::Region，创建内存中的Peer结构
	//将Peer按照取模法，分配到多个Store线程上
	for (size_t i = 0; i < regions.size(); ++i){
		uint64_t region_id = regions[i].id();
		int index = region_id % store_num_;
		Store* store = stores_[index];
		auto p = find_peer(regions[i], store->get_store_id());
		assert(p);
		Peer* peer = new Peer(db, store, regions[i], p->id());
		store->add_peer(region_id, peer);
	}

	//启动各个Store线程，每个Store线程负责若干Peer的消息
	for (int i = 0; i < store_num_; ++i){
		Store* store = stores_[i];
		store->init();
	}
	return true;
}

void StoreRouter::send_admin_request(metapb::Region region, metapb::Peer peer,
		raft_cmdpb::AdminRequest request) {
	auto region_id = region.id();
	auto cmd_type = request.cmd_type();

	uuid_t uuid;
	uuid_generate(uuid);
	char str[36];
	uuid_unparse(uuid, str);

	raft_cmdpb::RaftCmdRequest req;
	req.mutable_header()->set_region_id(region_id);
	req.mutable_header()->mutable_region_epoch()->CopyFrom(region.region_epoch());
	req.mutable_header()->mutable_peer()->CopyFrom(peer);
	req.mutable_header()->set_uuid(str);
	req.mutable_admin_request()->CopyFrom(request);
	this->on_command_message(req, boost::bind(&StoreRouter::empty_response, this, _1));
}

void StoreRouter::empty_response(const raft_cmdpb::RaftCmdResponse){
}

void StoreRouter::on_raft_message(const raft_serverpb::RaftMessage& raft){
	uint64_t range_id = raft.region_id();
	Store* cur_store = stores_[range_id % stores_.size()];
	cur_store->runInLoop(boost::bind(&Store::on_raft_message, cur_store, raft));
}

void StoreRouter::on_command_message(const raft_cmdpb::RaftCmdRequest& cmd, ResponseCallback callback){
	uint64_t range_id = cmd.header().region_id();
	Store* cur_store = stores_[range_id % stores_.size()];
	cur_store->runInLoop(boost::bind(&Store::on_command_message, cur_store, cmd, callback));
}

