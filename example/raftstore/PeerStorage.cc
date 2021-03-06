#include "PeerStorage.h"
#include "tikv_common.h"
#include "raft_serverpb.pb.h"
#include "metapb.pb.h"
#include "RawNode.h"
#include "keys.h"

const uint64_t RAFT_INIT_LOG_TERM = 1;
const uint64_t RAFT_INIT_LOG_INDEX = 5;
const uint64_t MAX_SNAP_TRY_CNT = 1;

// When we bootstrap the region or handling split new region, we must
// call this to initialize region local state first.
void write_initial_state(rocksdb::DB* db, rocksdb::WriteBatch& w , uint64_t region_id) {
	//RaftLocalState
	auto raft_state = raft_serverpb::RaftLocalState();
	raft_state.set_last_index(RAFT_INIT_LOG_INDEX);
	raft_state.mutable_hard_state()->set_term(RAFT_INIT_LOG_TERM);
	raft_state.mutable_hard_state()->set_commit(RAFT_INIT_LOG_INDEX);

	int raft_state_msg_size = raft_state.ByteSize();
	char* raft_state_buffer = new char[raft_state_msg_size];
	raft_state.SerializeToArray(raft_state_buffer, raft_state_msg_size);

	//RaftApplyState
	auto apply_state = raft_serverpb::RaftApplyState();
	apply_state.set_applied_index(RAFT_INIT_LOG_INDEX);
	apply_state.mutable_truncated_state()->set_index(RAFT_INIT_LOG_INDEX);
	apply_state.mutable_truncated_state()->set_term(RAFT_INIT_LOG_TERM);

	int apply_state_msg_size = apply_state.ByteSize();
	char* apply_state_buffer = new char[apply_state_msg_size];
	apply_state.SerializeToArray(apply_state_buffer, apply_state_msg_size);

	w.Put(raft_state_key(region_id), std::string(raft_state_buffer, raft_state_msg_size));
	//LOG_INFO << "raft_state_key key:" << raft_state_key(region_id) << " value:" << std::string(raft_state_buffer, raft_state_msg_size);
	w.Put(apply_state_key(region_id), std::string(apply_state_buffer, apply_state_msg_size));
	//LOG_INFO << "apply_state_key:" << apply_state_key(region_id) << " value:" << std::string(apply_state_buffer, apply_state_msg_size);

	delete []raft_state_buffer;
	delete []apply_state_buffer;
}

InvokeContext::InvokeContext(PeerStorage* store) {
	this->raft_state = store->get_raft_state();
	this->apply_state = store->get_apply_state();
	this->last_term = store->get_last_term();
}

void InvokeContext::save_raft(uint64_t region_id) {
	int raft_state_msg_size = this->raft_state.ByteSize();
	char* raft_state_buffer = new char[raft_state_msg_size + 1];
	raft_state_buffer[raft_state_msg_size] = '\0';
	this->raft_state.SerializeToArray(raft_state_buffer, raft_state_msg_size);
	this->wb.Put(raft_state_key(region_id), std::string(raft_state_buffer, raft_state_msg_size));
	delete []raft_state_buffer;
	LOG_INFO << "InvokeContext::save_raft:" << this->raft_state.DebugString();
}

void InvokeContext::save_apply(uint64_t region_id)  {
	int apply_state_msg_size = this->apply_state.ByteSize();
	char* apply_state_buffer = new char[apply_state_msg_size + 1];
	apply_state_buffer[apply_state_msg_size] = '\0';
	apply_state.SerializeToArray(apply_state_buffer, apply_state_msg_size);
	this->wb.Put(apply_state_key(region_id), std::string(apply_state_buffer, apply_state_msg_size));
	delete []apply_state_buffer;
	LOG_INFO << "InvokeContext::save_apply:" << this->apply_state.DebugString();
}

RaftState PeerStorage::initial_state(){
	auto hard_state = this->raft_state.hard_state();
	eraftpb::ConfState conf_state;

	if (hard_state.SerializeAsString() == eraftpb::HardState().SerializeAsString()) {
		//assert!(!self.is_initialized(),
		//		"peer for region {:?} is initialized but local state {:?} has empty hard \
		//		state",
		//		self.region,
		//		self.raft_state);
		RaftState state;
		state.hard_state = hard_state;
		state.conf_state = conf_state;
		return state;
	}

	for(auto& p :this->region.peers()) {
		conf_state.add_nodes(p.id());
	}
	RaftState state;
	state.hard_state = hard_state;
	state.conf_state = conf_state;
	return state;
	////message HardState {
	////	optional uint64 term   = 1; 
	////	optional uint64 vote   = 2; 
	////	optional uint64 commit = 3; 
	////}
	//eraftpb::HardState hard_state;
	//hard_state.CopyFrom(this->raft_state.hard_state());
	////hard_state.set_term(1);
	////hard_state.set_vote(4);
	////hard_state.set_commit(5);

	////message ConfState {
	////	repeated uint64 nodes = 1;
	////}
	//eraftpb::ConfState conf_state;
	//conf_state.add_nodes(4);
	//conf_state.add_nodes(5);
	//conf_state.add_nodes(6);

	//RaftState state;
	//state.hard_state = hard_state;
	//state.conf_state = conf_state;
	//return state;
}

//message HardState {
//	optional uint64 term   = 1; 
//	optional uint64 vote   = 2; 
//	optional uint64 commit = 3; 
//}
//
//message RaftLocalState {
//	optional eraftpb.HardState hard_state        = 1;
//	optional uint64 last_index                  = 2;
//}
raft_serverpb::RaftLocalState init_raft_state(rocksdb::DB* db, metapb::Region region)  {
	std::string value;
	rocksdb::Status s = db->Get(rocksdb::ReadOptions(), raft_state_key(region.id()), &value);
	if (value.size() != 0){
		typedef raft_serverpb::RaftLocalState T;
		std::string typeName = T::descriptor()->full_name();
		MessagePtr message;
		message.reset(createMessage(typeName));
		if (message) {
			const char* data = value.data();
			int32_t dataLen = value.size();
			if (message->ParseFromArray(data, dataLen)) {
				auto msg = muduo::down_pointer_cast<raft_serverpb::RaftLocalState>(message);
				LOG_INFO << "init_raft_state:" << msg->DebugString();
				return *msg;
			}else{
				LOG_FATAL << "ParseFromArray Error";
				exit(1);
			}
		}
	}
	auto raft_state = raft_serverpb::RaftLocalState();
	if (region.peers().size() > 0) {
		raft_state.set_last_index(RAFT_INIT_LOG_INDEX);
	}

	rocksdb::WriteBatch wb;
	//dev
	std::vector<eraftpb::Entry> entries;
	for(size_t i = 1; i <= RAFT_INIT_LOG_INDEX; ++i){
		eraftpb::Entry en;
		en.set_index(i);
		en.set_term(RAFT_INIT_LOG_TERM);
		entries.push_back(en);
	}
	for (size_t i = 0; i < entries.size(); ++i){
		eraftpb::Entry e = entries[i];
		int entry_msg_size = e.ByteSize();
		char* entry_buffer = new char[entry_msg_size + 1];
		entry_buffer[entry_msg_size] = '\0';
		e.SerializeToArray(entry_buffer, entry_msg_size);
		wb.Put(raft_log_key(region.id(), e.index()), std::string(entry_buffer, entry_msg_size));
		LOG_INFO << "append raftlog:[" << e.index() << "]" << " size:" << entry_msg_size;
		delete []entry_buffer;
	}
	s = db->Write(rocksdb::WriteOptions(), &wb);
	assert(s.ok());

	return raft_state;
}

//message RaftTruncatedState {
//	optional uint64 index    = 1;
//	optional uint64 term     = 2;
//}
//message RaftApplyState {
//	optional uint64 applied_index               = 1;
//	optional RaftTruncatedState truncated_state = 2;
//}
raft_serverpb::RaftApplyState init_apply_state(rocksdb::DB* db, metapb::Region region){
	std::string value;
	rocksdb::Status s = db->Get(rocksdb::ReadOptions(), apply_state_key(region.id()), &value);
	if (value.size() != 0){
		typedef raft_serverpb::RaftApplyState T;
		std::string typeName = T::descriptor()->full_name();
		MessagePtr message;
		message.reset(createMessage(typeName));
		if (message) {
			const char* data = value.data();
			int32_t dataLen = value.size();
			if (message->ParseFromArray(data, dataLen)) {
				auto msg = muduo::down_pointer_cast<raft_serverpb::RaftApplyState>(message);
				LOG_INFO << "init_apply_state:" << msg->DebugString();
				return *msg;
			}else{
				LOG_FATAL << "ParseFromArray Error";
				exit(1);
			}
		}
	}

	auto apply_state = raft_serverpb::RaftApplyState();
	if (region.peers().size() > 0) {
		apply_state.set_applied_index(RAFT_INIT_LOG_INDEX);
		auto state = apply_state.mutable_truncated_state();
		state->set_index(RAFT_INIT_LOG_INDEX);
		state->set_term(RAFT_INIT_LOG_TERM);
	}
	return apply_state;
}

uint64_t init_last_term(rocksdb::DB* db, metapb::Region region,
		raft_serverpb::RaftLocalState raft_state,
		raft_serverpb::RaftApplyState apply_state){
	uint64_t  last_idx = raft_state.last_index();
	if (last_idx == 0) {
		return 0;
	} else if (last_idx == RAFT_INIT_LOG_INDEX) {
		return RAFT_INIT_LOG_TERM;
	} else if (last_idx == apply_state.truncated_state().index() ){
		return apply_state.truncated_state().term();
	} else {
		assert(last_idx > RAFT_INIT_LOG_INDEX);
	}

	//从rocksdb中获取
	std::string last_log_key = raft_log_key(region.id(), last_idx);
	std::string value;
	rocksdb::Status s = db->Get(rocksdb::ReadOptions(), last_log_key, &value);
	assert(value.size() != 0);
	typedef eraftpb::Entry T;
	std::string typeName = T::descriptor()->full_name();
	MessagePtr message;
	message.reset(createMessage(typeName));
	assert(message != NULL);

	const char* data = value.data();
	int32_t dataLen = value.size();
	if (message->ParseFromArray(data, dataLen)) {
		auto msg = muduo::down_pointer_cast<eraftpb::Entry>(message);
		LOG_INFO << "init_last_term:" << msg->DebugString();
		return msg->term();
	}else{
		LOG_FATAL << "ParseFromArray Error";
		exit(1);
	}
	return 0;
}

metapb::Region PeerStorage::get_region(){
	return this->region;
}

PeerStorage::PeerStorage(rocksdb::DB* db_, metapb::Region region_) :
	db(db_),
	region(region_){
	this->raft_state = init_raft_state(db, region);
	LOG_INFO << "PeerStorage::PeerStorage load raft_state:" << this->raft_state.DebugString();
	this->apply_state = init_apply_state(db, region);
	LOG_INFO << "PeerStorage::PeerStorage load apply_state:" << this->apply_state.DebugString();
	this->last_term = init_last_term(db, region, raft_state, apply_state);
	LOG_INFO << "PeerStorage::PeerStorage load last_term:" << this->last_term;
	LOG_INFO << "PeerStorage::PeerStorage:" << this->raft_state.DebugString();	
}

PeerStorage::~PeerStorage(){
}

void PeerStorage::handle_raft_ready(Ready& ready){
	if (ready.entries.size() > 0)
		LOG_INFO << "PeerStorage::handle_raft_ready, entries_size:" << ready.entries.size();
	InvokeContext ctx(this);
	if (!ready.entries.empty()){
		this->append(ctx, ready.entries);
	}
	// Last index is 0 means the peer is created from raft message
	// and has not applied snapshot yet, so skip persistent hard state.
	if (ctx.raft_state.last_index() > 0 ){
		if (ready.hs) {
			ctx.raft_state.mutable_hard_state()->CopyFrom(ready.hs.get());
		}
	}

	if (ctx.raft_state.SerializeAsString() != this->raft_state.SerializeAsString()) {
		ctx.save_raft(this->get_region_id());
	}

	if (ctx.apply_state.SerializeAsString() != this->apply_state.SerializeAsString()) {
		ctx.save_apply(this->get_region_id());
	}

	auto s = db->Write(rocksdb::WriteOptions(), &ctx.wb);
	assert(s.ok());

	this->raft_state = ctx.raft_state;
	this->apply_state = ctx.apply_state;
	this->last_term = ctx.last_term;
}

uint64_t PeerStorage::first_index() const{
	return this->apply_state.truncated_state().index() + 1;
}

uint64_t PeerStorage::last_index() const {
	return this->raft_state.last_index();
}

uint64_t PeerStorage::truncated_term() const {
	return this->apply_state.truncated_state().term();
}

uint64_t PeerStorage::truncated_index() const {
	return this->apply_state.truncated_state().index();
}

Result<uint64_t, Error> PeerStorage::term(uint64_t idx) const{
	if (idx == this->truncated_index()) {
		return Ok(this->truncated_term());
	}
	int ret = this->check_range(idx, idx + 1);
	if (ret != 0){
		Error e = {"check_range error:"};
		return Err(e);
	}
	if (this->truncated_term() == this->last_term || idx == this->last_index()) {
		LOG_INFO << "PeerStorage::term, idx:[" << idx << "] " << this->last_term;
		return Ok(this->last_term);
	}
	std::string key = raft_log_key(this->get_region_id(), idx);
	std::string value;
	rocksdb::Status s = db->Get(rocksdb::ReadOptions(), key, &value);
	if (value.size() != 0){
		typedef eraftpb::Entry T;
		std::string typeName = T::descriptor()->full_name();
		MessagePtr message;
		message.reset(createMessage(typeName));
		assert(message != NULL);

		const char* data = value.data();
		int32_t dataLen = value.size();
		if (message->ParseFromArray(data, dataLen)) {
			auto msg = muduo::down_pointer_cast<eraftpb::Entry>(message);
			LOG_INFO << "PeerStorage::term, idx:[" << idx << "] " << msg->DebugString();
			return Ok(msg->term());
		}else{
			LOG_FATAL << "ParseFromArray Error";
			Error e = {"PTOTOBUG_PARSER_ERROR"};
			return Err(e);
		}
	}
	Error e = {"STORAGE_ERROR_UNAVABLE"};
	return Err(e);
}

int PeerStorage::check_range(uint64_t low, uint64_t high) const {
	if (low > high) {
		return STORAGE_RANGE_LOW_GREAT_HIGH_ERROR;
	} else if (low <= this->truncated_index()) {
		return RAFT_LOG_COMPACT_ERROR;
	} else if (high > this->last_index() + 1) {
		return RAFT_LOG_INDEX_OUT_OF_BOUND;
	}
	return 0;
}

// Append the given entries to the raft log using previous last index or self.last_index.
// Return the new last index for later update. After we commit in engine, we can set last_index
// to the return one.
//
void PeerStorage::append(InvokeContext& ctx, std::vector<eraftpb::Entry>& entries){
	LOG_INFO << " PeerStorage::append, " << entries.size();
	uint64_t prev_last_index = ctx.raft_state.last_index();

	LOG_INFO << "ctx.raft_state:" << ctx.raft_state.DebugString();
	if (entries.empty()) {
		return;
	}
	LOG_INFO << "PeerStorage::append," << entries[0].DebugString();

	for (size_t i = 0; i < entries.size(); ++i){
		eraftpb::Entry e = entries[i];
		int entry_msg_size = e.ByteSize();
		char* entry_buffer = new char[entry_msg_size + 1];
		entry_buffer[entry_msg_size] = '\0';
		e.SerializeToArray(entry_buffer, entry_msg_size);
		ctx.wb.Put(raft_log_key(this->get_region_id(), e.index()), std::string(entry_buffer, entry_msg_size));
		LOG_INFO << "append raftlog:[" << e.index() << "]" << " entry size:" << entry_msg_size;
		delete []entry_buffer;
	}

	eraftpb::Entry last_e = entries[entries.size() - 1];
	auto e_last_index = last_e.index();
	auto e_last_term = last_e.term();

	// Delete any previously appended log entries which never committed.
	for (uint64_t i = e_last_index + 1; i < prev_last_index + 1; ++i) {
		ctx.wb.Delete(raft_log_key(this->get_region_id(), i));
		LOG_INFO << "delete raftlog:[" << i << "]";
	}

    ctx.raft_state.set_last_index(e_last_index);
    ctx.last_term = e_last_term;
}

int PeerStorage::entries(uint64_t low, uint64_t high, uint64_t max_size,
		std::vector<eraftpb::Entry>& log_entries) const{
	assert(this->check_range(low, high) == 0);
	if (low == high) {
		return 0;
	}
	uint64_t total_size = 0;
	uint64_t next_index = low;

	std::string start_key = raft_log_key(this->get_region_id(), low);
	if (low + 1 == high) {
		// If election happens in inactive regions, they will just try
		// to fetch one empty log.
		std::string value;
		this->db->Get(rocksdb::ReadOptions(), start_key, &value);
		if (value.size() != 0){
			LOG_INFO << "PeerStorage::entries, index:[" << low << "] value_size:[" << value.size() << "]";
			//从rocksdb中获取
			typedef eraftpb::Entry T;
			std::string typeName = T::descriptor()->full_name();
			MessagePtr message;
			message.reset(createMessage(typeName));
			assert(message != NULL);

			const char* data = value.data();
			int32_t dataLen = value.size();
			if (message->ParseFromArray(data, dataLen)) {
				auto msg = muduo::down_pointer_cast<eraftpb::Entry>(message);
				log_entries.push_back(*msg);
				return 0;
			}else{
				LOG_FATAL << "ParseFromArray Error";
				exit(1);
			}
		}else{
			return STORAGE_ERROR_UNAVABLE;
		}
	}

	std::string end_key = raft_log_key(this->get_region_id(), high);

	rocksdb::Iterator* it = db->NewIterator(rocksdb::ReadOptions());
	it->Seek(start_key);
	for (; it->Valid(); it->Next()) {
		LOG_INFO << "Scan key_len:" << it->key().size() << " value_len:" << it->value().size();
		if (it->key().ToString() > end_key){
			break;
		}
		typedef eraftpb::Entry T;
		std::string typeName = T::descriptor()->full_name();
		MessagePtr message;
		message.reset(createMessage(typeName));
		assert(message != NULL);

		const char* data = it->value().data();
		int32_t dataLen = it->value().size();
		if (message->ParseFromArray(data, dataLen)) {
			auto entry = muduo::down_pointer_cast<eraftpb::Entry>(message);
			if (entry->index() != next_index) {
				LOG_INFO << "entry: " << entry->DebugString();
				return STORAGE_LOG_ENTRY_OFFSET_ERROR;
			}
			next_index += 1;
			total_size += it->value().size();
			log_entries.push_back(*entry);
		}else{
			LOG_FATAL << "ParseFromArray Error";
			exit(1);
		}
	}
	// If we get the correct number of entries the total size exceeds max_size, returns.
	if (log_entries.size() == high - low) {
		return 0;
	}
	// Here means we don't fetch enough entries.
	return STORAGE_ERROR_UNAVABLE;
}

