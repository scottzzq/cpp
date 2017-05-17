#include "Unstable.h"

uint64_t Unstable::maybe_first_index(){
	return INVALID_ID;
}

//优先取Unstable的entries中的
//offset:5
//5,6,7
//7
uint64_t Unstable::maybe_last_index(){
	if (this->offset + entries.size() - 1 > 0){
		return this->offset + entries.size() - 1;
	}
	return INVALID_ID;
}

//得到log编号是idx对应的Entry的term
//如果刚好是snapshot
uint64_t Unstable::maybe_term(uint64_t idx) {
	if (idx < this->offset) {
		return INVALID_ID;
	}
	uint64_t last = this->maybe_last_index();
	if (last != INVALID_ID){
		if (idx > last){
			return INVALID_ID;
		}
		return this->entries[idx - this->offset].term();
	}
	return INVALID_ID;
}

//offset:3
//3,4,5,6: 5
//start = 5 + 1 - 3 = 3

//offset:6
//已经持久化storage中，需要在unstable中删除
void Unstable::stable_to(uint64_t idx, uint64_t term){
	uint64_t t = this->maybe_term(idx);
	if (t == INVALID_ID){
		return;
	}

	if (t == term && idx >= this->offset) {
		auto start = idx + 1 - this->offset;
		for (size_t i = 0; i < start; ++i){
			this->entries.erase(this->entries.begin());
		}
		this->offset = idx + 1;
	}
}
//case1: 3
//3,4,5
//6,7,8

//case2: 3
//3,4,5
//1,2,3

//case3: 3
//3,4,5
//4,5,6
void Unstable::truncate_and_append(std::vector<eraftpb::Entry>& ents){
	LOG_INFO << "before truncate_and_append";
	for (size_t i = 0; i < this->entries.size(); ++i){
		LOG_INFO << this->entries[i].DebugString(); 
	}
	LOG_INFO << "end truncate_and_append";
	uint64_t after = ents[0].index();
	if (after == this->offset + this->entries.size()){
		for (size_t i = 0; i < ents.size(); ++i){
			this->entries.push_back(ents[i]);
		}
	}else if (after <= this->offset) {
		this->offset = after;
		this->entries.clear();
		for (size_t i = 0; i < ents.size(); ++i){
			this->entries.push_back(ents[i]);
		}
	}else{
		LOG_INFO << "truncate_and_append, before must_check_outofbounds, lo:" << this->offset << " hi:" << after;
		this->must_check_outofbounds(this->offset, after);
		for (size_t i = after - this->offset; i < this->entries.size(); ++i){
			this->entries.erase(this->entries.end() - 1);
		}
		for (size_t i = 0; i < ents.size(); ++i){
			this->entries.push_back(ents[i]);
		}
	}
	LOG_INFO << "after before truncate_and_append";
	for (size_t i = 0; i < this->entries.size(); ++i){
		LOG_INFO << this->entries[i].DebugString(); 
	}
	LOG_INFO << "after end truncate_and_append";
}

//offset:2
//2,3,4,5,6
//3,4
//3 - 2 = 1
//4 - 2 = 2
std::vector<eraftpb::Entry> Unstable::slice(uint64_t lo, uint64_t hi) {
	this->must_check_outofbounds(lo, hi);
	auto l = lo;
	auto h = hi;
	auto off = this->offset;
	std::vector<eraftpb::Entry> res;
	for (size_t i = l - off; i < h - off; ++i){
		res.push_back(this->entries[i]);
	}
	return res;
}

//检查是否在合法范围之内
void Unstable::must_check_outofbounds(uint64_t lo, uint64_t hi) {
	LOG_INFO << "lo:" << lo << " hi:" << hi;
	if (lo > hi) {
		assert(lo <= hi);
		//panic!("{} invalid unstable.slice {} > {}", self.tag, lo, hi)
	}
	auto upper = this->offset + this->entries.size();
	if (lo < this->offset || hi > upper) {
		assert(lo >= this->offset && hi <= upper);
		//panic!("{} unstable.slice[{}, {}] out of bound[{}, {}]",
		//		self.tag,
		//		lo,
		//		hi,
		//		self.offset,
		//		upper)
	}
}
