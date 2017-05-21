#include "StoreRouter.h"
#include "TiKVServer.h"

#include <sstream>
#include <boost/bind.hpp>
#include "Peer.h"
#include "keys.h"
#include "tikv_common.h"

const uint64_t INIT_EPOCH_VER = 1;
const uint64_t INIT_EPOCH_CONF_VER = 1;

void write_region(rocksdb::DB* db, metapb::Region& region) {
	auto state = raft_serverpb::RegionLocalState();
	state.mutable_region()->CopyFrom(region);

	rocksdb::WriteBatch wb;

	int msg_size = state.ByteSize();
	char* buffer = new char[msg_size + 1];
	buffer[msg_size] = '\0';
	state.SerializeToArray(buffer, msg_size);

	wb.Put(region_state_key(region.id()), buffer);

	//for dev
	std::string value;
	db->Get(rocksdb::ReadOptions(), raft_state_key(region.id()), &value);
	if (value.size() == 0){
		LOG_INFO << "write_region,write_initial_state";
		write_initial_state(db, wb, region.id());

		auto s = db->Write(rocksdb::WriteOptions(), &wb);
		assert(s.ok());

		LOG_INFO << "write_region:" << region.DebugString() << 
			" key_len:" << region_state_key(region.id()).size() << " value_len:" << msg_size;
	}

	delete []buffer;
}

// Bootstrap first region.
metapb::Region bootstrap_region(rocksdb::DB* db) {
	metapb::Peer peer1;
	peer1.set_id(4);
	peer1.set_store_id(1);

	metapb::Peer peer2;
	peer2.set_id(5);
	peer2.set_store_id(2);

	metapb::Peer peer3;
	peer3.set_id(6);
	peer3.set_store_id(3);

	metapb::Region region;
	metapb::RegionEpoch* epoch = region.mutable_region_epoch();
	epoch->set_conf_ver(INIT_EPOCH_VER);
	epoch->set_version(INIT_EPOCH_CONF_VER);

	region.set_id(1);
	region.set_term(1);

	metapb::Peer* p1 = region.add_peers();
	p1->CopyFrom(peer1);
	metapb::Peer* p2 = region.add_peers();
	p2->CopyFrom(peer2);
	metapb::Peer* p3 = region.add_peers();
	p3->CopyFrom(peer3);
	write_region(db, region);
	return region;
}

StoreRouter::StoreRouter(TiKVServer* server, int store_num): server_(server),store_num_(store_num){
	stores_.clear();
	rocksdb::Options options;
	options.create_if_missing = true;
	rocksdb::Status status = rocksdb::DB::Open(options, "./db", &db);
	assert(status.ok());

	//develop
	// Case insensitive key/value for replica constraints.
	//message StoreLabel {
	//	optional string key         = 1 [(gogoproto.nullable) = false];
	//	optional string value       = 2 [(gogoproto.nullable) = false];
	//}
	//message Store {
	//	optional uint64 id          = 1 [(gogoproto.nullable) = false];
	//	optional string address     = 2 [(gogoproto.nullable) = false];
	//	optional StoreState state   = 3 [(gogoproto.nullable) = false];
	//	repeated StoreLabel labels  = 4;
	//	// more attributes......
	//}
	slash::BaseConf* conf = TiKVConfig::getInstance()->getBaseConf();
	int store_id;
	conf->GetConfInt("store_id", &store_id);

	std::string address;
	conf->GetConfStr("address", &address);

	store.set_id(store_id);
	store.set_address(address);
	bootstrap_region(db);
}

StoreRouter::~StoreRouter(){
	for(int i = 0; i < store_num_; ++i){
		delete stores_[i];
	}
	stores_.clear();
}

bool StoreRouter::init(){
	for (int i = 0; i < store_num_; ++i){
		std::stringstream ss;
		ss << i;
		std::string thread_name = "Store-";
		ss >> thread_name;
		Store* store = new Store(server_, this->store.id());
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
		LOG_INFO << "Scan key_len:" << it->key().size() << " value_len:" << it->value().size();
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
		Peer* peer = new Peer(db, store, regions[i]);
		store->add_peer(region_id, peer);
	}

	//启动各个Store线程，每个Store线程负责若干Peer的消息
	for (int i = 0; i < store_num_; ++i){
		Store* store = stores_[i];
		store->init();
	}
	return true;
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

