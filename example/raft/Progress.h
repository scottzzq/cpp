#ifndef PROGRESS_H
#define PROGRESS_H

#include <vector>
#include <string>
#include <assert.h>

enum ProgressState {
	Probe,
	Replicate,
	Snapshot,
};

class Inflights {
	public:
		Inflights(uint64_t cap){
			this->buffer.reserve(cap);
			this->count = 0;
			this->start = 0;
		}

		~Inflights(){
		}

		// full returns true if the inflights is full.
		inline bool full() const{
			return this->count == this->cap();
		}

		inline uint64_t cap() const{
			return this->buffer.capacity();
		}
		void add(uint64_t inflight);

		void free_to(uint64_t to);
		inline void free_first_one() {
			auto s = this->buffer[this->start];
			this->free_to(s);
		}

		// resets frees all inflights.
		inline void reset() {
			this->count = 0;
			this->start = 0;
		}
		// the starting index in the buffer
		uint64_t start;
		// number of inflights in the buffer
		uint64_t count;

		// ring buffer
		std::vector<uint64_t> buffer;
};

class Progress{
	public:
		Progress(uint64_t next_idx_, uint64_t ins_size):
			next_idx(next_idx_), 
			matched(0),
			state(ProgressState::Probe),
			paused(false),
			pending_snapshot(0),
			ins(ins_size) {
			}
		~Progress(){
		}

		void reset_state(ProgressState state){
			this->state = state;
			this->paused = false;
			this->pending_snapshot = 0;
			this->ins.reset();
		}

		//状态变为Probe
		//1.当前处于Snapshot，
		//2.当前处于其他状态
		inline void become_probe() {
			// If the original state is ProgressStateSnapshot, progress knows that
			// the pending snapshot has been sent to this peer successfully, then
			// probes from pendingSnapshot + 1.
			if (this->state == ProgressState::Snapshot) {
				auto pending_snap = this->pending_snapshot;
				this->reset_state(ProgressState::Probe);
				this->next_idx = std::max(this->matched + 1, pending_snap + 1);
			} else {
				this->reset_state(ProgressState::Probe);
				this->next_idx = this->matched + 1;
			}
		}

		//改变状态为Replicate
		inline void become_replicate() {
			this->reset_state(ProgressState::Replicate);
			this->next_idx = this->matched + 1;
		}

		//改变状态为Snapshot
		inline void become_snapshot(uint64_t snapshot_idx) {
			this->reset_state(ProgressState::Snapshot);
			this->pending_snapshot = snapshot_idx;
		}

		void snapshot_failure() {
			this->pending_snapshot = 0;
		}

		// maybe_snapshot_abort unsets pendingSnapshot if Match is equal or higher than
		// the pendingSnapshot
		bool maybe_snapshot_abort() {
			return this->state == ProgressState::Snapshot && this->matched >= this->pending_snapshot;
		}

		inline void resume() {
			this->paused = false;
		}

		//尝试更新进度
		bool maybe_update(uint64_t n);

		// maybe_decr_to returns false if the given to index comes from an out of order message.
		// Otherwise it decreases the progress next index to min(rejected, last) and returns true.
		//对于进度拉下太大的Follower，刚开始next发送过去，是不能匹配到的，所以需要
		//返回false说明乱序的消息
		//1、处于Replicate状态，直接将next_idx设置为matched + 1
		//2、处于其他状态next_idx设置为min(rejected, last + 1)
		bool maybe_decr_to(uint64_t rejected, uint64_t last);

		inline void optimistic_update(uint64_t n) {
			this->next_idx = n + 1;
		}

		//1、当处于Probe状态，判断paused状态
		//2、当处于Replicate状态，环形缓冲区已经满了
		//3、处于Snapshot状态
		inline bool is_paused(){
			switch(this->state) {
				case ProgressState::Probe:
					return paused;
				case ProgressState::Replicate:
					return this->ins.full();
				case ProgressState::Snapshot:
					return true;
			}
			return true;
		}

		inline void pause() {
			this->paused = true;
		}

		//log的下一个idx
		uint64_t next_idx;
		//Follower已知最大的idx
		uint64_t matched;
		// When in ProgressStateProbe, leader sends at most one replication message
		// per heartbeat interval. It also probes actual progress of the follower.
		//
		// When in ProgressStateReplicate, leader optimistically increases next
		// to the latest entry sent after sending replication message. This is
		// an optimized state for fast replicating log entries to the follower.
		//
		// When in ProgressStateSnapshot, leader should have sent out snapshot
		// before and stops sending any replication message.
		ProgressState state;
		// Paused is used in ProgressStateProbe.
		// When Paused is true, raft should pause sending replication message to this peer.
		bool paused;
		// pending_snapshot is used in ProgressStateSnapshot.
		// If there is a pending snapshot, the pendingSnapshot will be set to the
		// index of the snapshot. If pendingSnapshot is set, the replication process of
		// this Progress will be paused. raft will not resend snapshot until the pending one
		// is reported to be failed.
		uint64_t pending_snapshot;

		// recent_active is true if the progress is recently active. Receiving any messages
		// from the corresponding follower indicates the progress is active.
		// RecentActive can be reset to false after an election timeout.
		bool recent_active;

		// Inflights is a sliding window for the inflight messages.
		// When inflights is full, no more message should be sent.
		// When a leader sends out a message, the index of the last
		// entry should be added to inflights. The index MUST be added
		// into inflights in order.
		// When a leader receives a reply, the previous inflights should
		// be freed by calling inflights.freeTo.
		Inflights ins;
};

#endif
