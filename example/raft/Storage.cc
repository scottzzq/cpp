#include "Storage.h"
#include <vector>
#include "tikv_common.h"
#include <cstdlib>

//develop
RaftState MemStorage::initial_state(){
	//message HardState {
	//	optional uint64 term   = 1; 
	//	optional uint64 vote   = 2; 
	//	optional uint64 commit = 3; 
	//}
	eraftpb::HardState hard_state;
	//hard_state.set_term(1);
	//hard_state.set_vote(1);
	//hard_state.set_commit(1);

	//message ConfState {
	//	repeated uint64 nodes = 1;
	//}
	eraftpb::ConfState conf_state;
	conf_state.add_nodes(4);
	conf_state.add_nodes(5);
	conf_state.add_nodes(6);

	RaftState state;
	state.hard_state = hard_state;
	state.conf_state = conf_state;
	return state;
}

int MemStorage::entries(uint64_t low, uint64_t high, uint64_t max_size, 
		std::vector<eraftpb::Entry>& log_entries) const{
	uint64_t offset = this->raft_log_entries[0].index();
	if (low <= offset) {
		return RAFT_LOG_COMPACT_ERROR;
	}

	uint64_t last_idx = this->inner_last_index();
	if (high > last_idx + 1){
		//TODO:
		//panic!("index out of bound")
		return RAFT_LOG_INDEX_OUT_OF_BOUND;
	}
	// only contains dummy entries.
	if (this->raft_log_entries.size() == 1){
		return RAFT_LOG_EMPTY_UNAVABLE;
	}

	uint64_t lo = low - offset;
	uint64_t hi = high - offset;
	for (size_t i = lo; i < hi; ++i){
		log_entries.push_back(this->raft_log_entries[i]);
	}
	//TODO:max_size
	return 0;
}

int MemStorage::term(uint64_t idx, uint64_t& t) const {
	for (size_t i = 0; i < this->raft_log_entries.size(); ++i){
		LOG_INFO << "MemStorage::term:  idx:[" << i << "]" 
			<< this->raft_log_entries[i].DebugString();
	}
	uint64_t offset = this->raft_log_entries[0].index();
	if (idx < offset) {
		return RAFT_LOG_COMPACT_ERROR;
	}
	if (idx - offset >= this->raft_log_entries.size()){
		return RAFT_LOG_EMPTY_UNAVABLE;
	}
	t = this->raft_log_entries[(idx - offset)].term();
	return 0;
}

uint64_t MemStorage::first_index() const{
	return this->raft_log_entries[0].index() + 1;
}

uint64_t MemStorage::inner_last_index() const{
	return this->raft_log_entries[0].index() + this->raft_log_entries.size() - 1;
}

uint64_t MemStorage::last_index() const{
	LOG_INFO << "last_index:";
	for (size_t i = 0; i < this->raft_log_entries.size(); ++i)	{
		LOG_INFO << "array:" << i << " index:[" << this->raft_log_entries[i].index() << "]" 
			<< " term:[" << this->raft_log_entries[i].term() << "]";
	}
	LOG_INFO << "last_index:";
	return inner_last_index();
}

void MemStorage::append(std::vector<eraftpb::Entry>& ents){
	LOG_INFO << "before append:";
	for (size_t i = 0; i < this->raft_log_entries.size(); ++i)	{
		LOG_INFO << "array:" << i << " index:[" << this->raft_log_entries[i].index() << 
			"] term:[" << this->raft_log_entries[i].term() << "]";
	}
	LOG_INFO << "end append:";

	LOG_INFO << "start new append:";
	for (size_t i = 0; i < ents.size(); ++i){
		LOG_INFO << "array:" << i << " index:[" << ents[i].index() 
			<< "] term:[" << ents[i].term() << "]";
	}
	LOG_INFO << "end new append:";

	if (ents.empty()) {
		return;
	}
	uint64_t first = this->raft_log_entries[0].index() + 1;
	uint64_t last = ents[0].index() + ents.size() - 1;
	//需要append的log的index比当前最小的log index还小是不允许的
	if (last < first) {
		return;
	}

	// 可能会有冲突，需要append的log 已经被compact过
	// case1,需要删除冲突的部分
	// 持久化log: 4|,5,6       ---->first=5
	// append log: 2,3,4,5,6,7 ---->2
	// start = (5 - 2) = 3 
	//
	// case2，不需要特殊处理
	// 持久化log: 4|,5,6       ---->first=5
	// append log: 5,6,7       ---->5
	//
	// case3,不需要特殊处理
	// 持久化log: 4|,5,6       ---->first=5
	// append log: 6,7         ---->6
	if (first > ents[0].index()){
		uint64_t start = first - ents[0].index();
		//删除前面几个Log Entry
		for (size_t i = 0; i < start; ++i){
			std::vector<eraftpb::Entry>::iterator it = ents.begin();
			ents.erase(it);
		}
	}

	//case1:
	//持久化log: 0,|1,2,3
	//append log: 3,4,5
	//offset: 3 - 0 = 3
	//size: 4

	//case2:
	//持久化log: 0,|1,2,3
	//append log: 4,5,6
	//offset: 4 - 0 = 4
	//size: 4

	//case3:
	//持久化log: 0,|1,2,3
	//append log: 5,6,7
	//offset: 5 - 0 = 5
	//size:4
	uint64_t offset = ents[0].index() - this->raft_log_entries[0].index();
	if (this->raft_log_entries.size() > offset) {
		std::vector<eraftpb::Entry> new_entries;
		for (size_t i = 0; i < offset; ++i){
			new_entries.push_back(this->raft_log_entries[i]);
		}
		for (size_t i = 0; i < ents.size(); ++i){
			new_entries.push_back(ents[i]);
		}
		this->raft_log_entries = new_entries;
	}else if (this->raft_log_entries.size() == offset){
		for (size_t i = 0; i < ents.size(); ++i){
			this->raft_log_entries.push_back(ents[i]);
		}
	}else{
		char log[1024];
		snprintf(log, sizeof(log), "missing log entry [last: %lu, append at: %lu]",
				this->inner_last_index(),
				ents[0].index());
		LOG_FATAL << log;
		abort();
	}

	LOG_INFO << "after append:";
	for (size_t i = 0; i < this->raft_log_entries.size(); ++i)	{
		LOG_INFO << "array:" << i << " index:[" << this->raft_log_entries[i].index() << "]" << " term:[" << this->raft_log_entries[i].term() << "]";
	}
	LOG_INFO << "after append:";


}
//eraftpb::Snapshot MemStorage::snapshot(){
//}

