#ifndef STORAGE_H
#define STORAGE_H

#include "eraftpb.pb.h"
#include <vector>

struct RaftState {
	eraftpb::HardState hard_state;
	eraftpb::ConfState conf_state;
};

class Storage {
	public: 
		Storage(){
		}
		virtual ~Storage(){
		}
		/// initial_state returns the RaftState information
		virtual RaftState initial_state() = 0;
		virtual void append(std::vector<eraftpb::Entry>& ents) = 0;
		/// entries returns a slice of log entries in the range [lo,hi).
		/// max_size limits the total size of the log entries returned, but
		/// entries returns at least one entry if any.
		virtual int entries(uint64_t low, uint64_t high, uint64_t max_size, 
				std::vector<eraftpb::Entry>& log_entries) const = 0;
		/// term returns the term of entry idx, which must be in the range
		/// [first_index()-1, last_index()]. The term of the entry before
		/// first_index is retained for matching purpose even though the
		/// rest of that entry may not be available.
		virtual int term(uint64_t idx, uint64_t& t) const = 0;
		/// first_index returns the index of the first log entry that is
		/// possible available via entries (older entries have been incorporated
		/// into the latest snapshot; if storage only contains the dummy entry the
		/// first log entry is not available).
		virtual uint64_t first_index() const = 0;
		/// last_index returns the index of the last entry in the log.
		virtual uint64_t last_index() const = 0;
		/// snapshot returns the most recent snapshot.
		/// If snapshot is temporarily unavailable, it should return SnapshotTemporarilyUnavailable,
		/// so raft state machine could know that Storage needs some time to prepare
		/// snapshot and call snapshot later.
		//virtual eraftpb::Snapshot snapshot() = 0;
};

class MemStorage : public Storage{
	public:
		MemStorage(){
			this->raft_log_entries.push_back(eraftpb::Entry());
		}
		virtual ~MemStorage(){
		}
		virtual RaftState initial_state();
		virtual void append(std::vector<eraftpb::Entry>& ents);
		virtual int entries(uint64_t low, uint64_t high, uint64_t max_size, 
				std::vector<eraftpb::Entry>& log_entries) const;
		virtual int term(uint64_t idx, uint64_t& t) const;
		virtual uint64_t first_index() const;
		virtual uint64_t last_index() const;
		//virtual eraftpb::Snapshot snapshot();
	private:
		uint64_t inner_last_index() const;
		eraftpb::HardState hard_state;
		eraftpb::Snapshot snapshot;
		//entries[i]代表i+snapshot.get_metadata().get_index()对应位置的log
		//entries[0]不能用，初始状态放一个空的Entry到entries[0]
		//apply snapshot的时候，放置apply_index apply_term：apply_index之前所有的数据
		std::vector<eraftpb::Entry> raft_log_entries;
};

#endif
