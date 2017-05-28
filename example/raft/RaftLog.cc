#include "RaftLog.h"
#include "Storage.h"
#include "tikv_common.h"

RaftLog::RaftLog(Storage* store_):store(store_){
	uint64_t first_index = store->first_index();
	uint64_t last_index = store->last_index();

	this->committed = first_index - 1;
	this->applied = first_index - 1;

	LOG_INFO << "first_index:" << first_index << " last_index:" << last_index;
	this->unstable = new Unstable(last_index + 1, tag);
}

RaftLog::~RaftLog(){
}

std::vector<eraftpb::Entry> RaftLog::unstable_entries() {
	std::vector<eraftpb::Entry> res;
	if (this->unstable->entries.empty()) {
		return res;
	}
	return this->unstable->entries;
}

//优先使用unstable的
//当unstable没有的时候，选用storage中的
//初始化为：1
uint64_t RaftLog::first_index(){
	if (this->unstable->maybe_first_index() != INVALID_ID){
		return this->unstable->maybe_first_index();
	}
	return store->first_index();
}

//优先使用unstable的
//当unstable没有的时候，选用storage中的
//初始化为：0
uint64_t RaftLog::last_index(){
	if (this->unstable->maybe_last_index() != INVALID_ID){
		return this->unstable->maybe_last_index();
	}
	return store->last_index();
}

int RaftLog::term(uint64_t idx, uint64_t& t) {
	// the valid term range is [index of dummy entry, last index]
	auto dummy_idx = this->first_index() - 1;
	if (idx < dummy_idx || idx > this->last_index()) {
		t = 0;
		return 0;
	}
	if (this->unstable->maybe_term(idx) != INVALID_ID){
		t = this->unstable->maybe_term(idx);
		return 0;
	}
	int ret = store->term(idx, t);
	LOG_INFO << "RaftLog::term, idx:[" << idx << "] term:[" << t << "]"; 
	return ret;
}

uint64_t RaftLog::last_term(){
	uint64_t t = 0;
	int res = this->term(this->last_index(), t);
	assert(res == 0);
	return t;
} 

bool RaftLog::match_term(uint64_t idx, uint64_t term) {
	uint64_t t;
	if (this->term(idx, t) == 0){
		if (t == term){
			return true;
		}
	}
	return false;
}

bool RaftLog::maybe_commit(uint64_t max_index, uint64_t term){
	if (max_index > this->committed) {
		uint64_t t;
		int ret = this->term(max_index, t);
		assert(ret == 0);
		if (t == term) {
			this->commit_to(max_index);
			return true;
		}
	} 
	return false;
}

void RaftLog::commit_to(uint64_t to_commit) {
	// never decrease commit
	if (this->committed >= to_commit) {
		return;
	}
	if (this->last_index() < to_commit) {
		char log[1024];
		snprintf(log, sizeof(log), "%s to_commit %lu is out of range [last_index %lu]",
				this->tag.c_str(),
				to_commit,
				this->last_index());
		LOG_FATAL << log;
		exit(1);
	}
	this->committed = to_commit;
	LOG_INFO << "RaftLog::commit_to:" << to_commit;
}

void RaftLog::applied_to(uint64_t idx){
	if (idx == 0) {
		return;
	}
	if (this->committed < idx || idx < this->applied) {
		LOG_FATAL << this->tag << "applied(" << idx << ") is out of range [prev_applied(" 
			<< this->applied <<"), committed(" << this->committed <<")";
		abort();
		exit(1);
	}
	if (this->applied != idx)
		LOG_INFO << "apply to:" << idx;
	this->applied = idx;
}

void RaftLog::stable_to(uint64_t idx, uint64_t term){
	LOG_INFO << "RaftLog::stable_to, idx:[" << idx << "] term:[" << term << "]";
	this->unstable->stable_to(idx, term);
}

// find_conflict finds the index of the conflict.
// It returns the first index of conflicting entries between the existing
// entries and the given entries, if there are any.
// If there is no conflicting entries, and the existing entries contain
// all the given entries, zero will be returned.
// If there is no conflicting entries, but the given entries contains new
// entries, the index of the first new entry will be returned.
// An entry is considered to be conflicting if it has the same index but
// a different term.
// The first entry MUST have an index equal to the argument 'from'.
// The index of the given entries MUST be continuously increasing.
uint64_t RaftLog::find_conflict(const google::protobuf::RepeatedPtrField<eraftpb::Entry>& ents){
	LOG_INFO << "find_conflict, size:" << ents.size();
	for (auto& e: ents) {
		LOG_INFO << "find_conflict:" << e.DebugString();

		if (!this->match_term(e.index(), e.term())) {
			if (e.index() <= this->last_index()) {
				uint64_t t;
				int ret = this->term(e.index(), t);
				LOG_INFO << this->tag << " found conflict at index " << e.index() << 
					", [existing term:"<< t << ", conflicting term:" << e.term() << "]";
			}
			return e.index();
		}
	}
	return 0;
}

// maybe_append returns None if the entries cannot be appended. Otherwise,
// it returns Some(last index of new entries).
uint64_t RaftLog::maybe_append(uint64_t idx,
		uint64_t term,
		uint64_t committed,
		const google::protobuf::RepeatedPtrField<eraftpb::Entry>& ents){
	uint64_t last_new_index = idx + ents.size();
	if (this->match_term(idx, term)) {
		uint64_t conflict_idx = this->find_conflict(ents);
		LOG_INFO  << " conflict_idx:" << conflict_idx;
		if (conflict_idx == 0) {

		} else if (conflict_idx <= this->committed ){
			char log[1024];
			snprintf(log, sizeof(log), "%s entry %lu conflict with committed entry %lu",
					this->tag.c_str(),
					conflict_idx,
					this->committed);
			LOG_FATAL << log;
			exit(1);
		} else {
			uint64_t offset = idx + 1;
			LOG_INFO << " RaftLog::maybe_append, conflict_idx:" << conflict_idx << " offset:" << offset;
			this->append(ents, conflict_idx - offset);
		}
		this->commit_to(std::min(committed, last_new_index));
		return last_new_index;
	}
	return 0;
}

uint64_t RaftLog::append(std::vector<eraftpb::Entry>& es){
	if (es.empty()){
		return this->last_index();
	}
	uint64_t after = es[0].index() - 1;
	if (after < this->committed){
		char log[1024];
		snprintf(log, sizeof(log), "%s after %lu is out of range [committed %lu]",
				this->tag.c_str(),
				after,
				this->committed);
		LOG_FATAL << log;
		exit(1);
	}
	this->unstable->truncate_and_append(es);
	return last_index();
}

uint64_t RaftLog::append(const google::protobuf::RepeatedPtrField<eraftpb::Entry>& ents, int start){
	assert(start < ents.size());
	uint64_t after = ents.Get(start).index() - 1;
	if (after < this->committed) {
		char log[1024];
		snprintf(log, sizeof(log), "%s after %lu is out of range [committed %lu]",
				this->tag.c_str(),
				after,
				this->committed);
		LOG_FATAL << log;
	}
	std::vector<eraftpb::Entry> ens;
	for(int i = start; i < ents.size(); ++i){
		ens.push_back(ents.Get(i));
	}
	this->unstable->truncate_and_append(ens);
	return this->last_index();
}

std::vector<eraftpb::Entry> RaftLog::entries(uint64_t idx, uint64_t max_size){
	uint64_t last = this->last_index();
	LOG_INFO << "last:" << last << " idx:" << idx;
	if (idx > last) {
		return std::vector<eraftpb::Entry>();
	}
	return this->slice(idx, last + 1, max_size);
}

std::vector<eraftpb::Entry> RaftLog::all_entries(){
	return std::vector<eraftpb::Entry>();
}

int RaftLog::must_check_outofbounds(uint64_t low, uint64_t high) {
	if (low > high) {
		//TODO:
		assert(low <= high);
		//panic!("{} invalid slice {} > {}", self.tag, low, high)
	}

	uint64_t first_index = this->first_index();
	if (low < first_index) {
		return RAFT_LOG_COMPACT_ERROR;
	}

	uint64_t length = this->last_index() + 1 - first_index;
	if (low < first_index || high > first_index + length){
		//TODO:
		assert(low >= first_index);
		assert(high <= first_index + length);
		//panic!("{} slice[{},{}] out of bound[{},{}]",
		//		self.tag,
		//		low,
		//		high,
		//		first_index,
		//		self.last_index())
	}
	return 0;
}

std::vector<eraftpb::Entry> RaftLog::slice(uint64_t low, uint64_t high, uint64_t max_size){
	LOG_INFO << "RaftLog::slice low:" << low << " high:" << high;
	int check_bound_res = this->must_check_outofbounds(low, high);
	assert(check_bound_res == 0);
	std::vector<eraftpb::Entry> entries;
	if (low == high) {
		return entries;
	}

	if (low < this->unstable->offset) {
		int ret = this->store->entries(low, std::min(high, this->unstable->offset), max_size, entries);
		//TODO
		if (ret != 0){
			LOG_INFO << "ret:" << ret << " size:" << entries.size();
			exit(1);
		}
		assert(ret == 0);
		if (low + entries.size() < std::min(high, this->unstable->offset)) {
			return entries;
		}
	}

	if (high > this->unstable->offset) {
		auto unstable_ents = this->unstable->slice(std::max(low, this->unstable->offset), high);
		for (size_t i = 0; i < unstable_ents.size(); ++i){
			entries.push_back(unstable_ents[i]);
		}
	}
	return entries;
}

// is_up_to_date determines if the given (lastIndex,term) log is more up-to-date
// by comparing the index and term of the last entry in the existing logs.
// If the logs have last entry with different terms, then the log with the
// later term is more up-to-date. If the logs end with the same term, then
// whichever log has the larger last_index is more up-to-date. If the logs are
// the same, the given log is up-to-date.
bool RaftLog::is_up_to_date(uint64_t last_index, uint64_t term){
	return term > this->last_term() || (term == this->last_term() && last_index >= this->last_index());
}

std::vector<eraftpb::Entry> RaftLog::next_entries(){
	uint64_t offset = std::max(this->applied + 1, this->first_index());
	if (this->committed + 1 > offset) {
		return this->slice(offset, committed + 1, NO_LIMIT);
	}
	return std::vector<eraftpb::Entry>();
}

//用来判断当前是否有可以写入到Storage中的log
bool RaftLog::has_next_entries(){
	auto offset = std::max(this->applied + 1, this->first_index());
	return this->committed + 1 > offset;
}

