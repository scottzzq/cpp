#include "Progress.h"

// add adds an inflight into inflights
void Inflights::add(uint64_t inflight) {
	if (this->full()) {
		assert(!this->full());
		//panic!("cannot add into a full inflights")
	}

	uint64_t next = this->start + this->count;
	if (next >= this->cap()) {
		next -= this->cap();
	}
	assert(next <= this->buffer.size());

	if (next == this->buffer.size()) {
		this->buffer.push_back(inflight);
	} else {
		this->buffer[next] = inflight;
	}
	this->count += 1;
}

// free_to frees the inflights smaller or equal to the given `to` flight.
void Inflights::free_to(uint64_t to){
	if (this->count == 0 || to < this->buffer[this->start]) {
		// out of the left side of the window
		return;
	}

	uint64_t i = 0;
	uint64_t idx = this->start;
	while (i < this->count) {
		if (to < this->buffer[idx]){
			// found the first large inflight
			break;
		}
		// increase index and maybe rotate
		idx += 1;
		if (idx >= this->cap()) {
			idx -= this->cap();
		}
		i += 1;
	}

	// free i inflights and set new start index
	this->count -= i;
	this->start = idx;
}

//尝试更新进度
bool Progress::maybe_update(uint64_t n){
	//当前id的进度是否比n小
	bool need_update = this->matched < n;
	if (need_update) {
		//更新进度到n
		this->matched = n;
		this->resume();
	}

	if (this->next_idx < n + 1) {
		this->next_idx = n + 1;
	}
	return need_update;
}

// maybe_decr_to returns false if the given to index comes from an out of order message.
// Otherwise it decreases the progress next index to min(rejected, last) and returns true.
//对于进度拉下太大的Follower，刚开始next发送过去，是不能匹配到的，所以需要
//返回false说明乱序的消息
//1、处于Replicate状态，直接将next_idx设置为matched + 1
//2、处于其他状态next_idx设置为min(rejected, last + 1)
bool Progress::maybe_decr_to(uint64_t rejected, uint64_t last) {
	if (this->state == ProgressState::Replicate) {
		// the rejection must be stale if the progress has matched and "rejected"
		// is smaller than "match".
		if (rejected <= this->matched) {
			return false;
		}
		this->next_idx = this->matched + 1;
		return true;
	}

	// the rejection must be stale if "rejected" does not match next - 1
	if (this->next_idx == 0 || this->next_idx - 1 != rejected) {
		return false;
	}

	this->next_idx = std::min(rejected, last + 1);
	if (this->next_idx < 1) {
		this->next_idx = 1;
	}
	this->resume();
	return true;
}
