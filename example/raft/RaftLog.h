#ifndef RAFTLOG_H
#define RAFTLOG_H

#include <string>
#include "eraftpb.pb.h"
#include "Unstable.h"
#include "tikv_common.h"

class Storage;

class RaftLog {
	public:
		RaftLog(Storage* store_);
		~RaftLog();

		std::vector<eraftpb::Entry> unstable_entries();
		uint64_t first_index();
		uint64_t last_index();

		int term(uint64_t idx, uint64_t& t);
		uint64_t last_term();

		const Storage* get_store(){
			return store;
		}
		Storage* mut_store(){
			return store;	
		}

		bool match_term(uint64_t idx, uint64_t term);
		bool maybe_commit(uint64_t max_index, uint64_t term);

		void commit_to(uint64_t to_commit);
		void applied_to(uint64_t to);

		inline uint64_t get_applied(){
			return this->applied;
		}
		void stable_to(uint64_t idx, uint64_t term);

		Unstable* get_unstable(){
			return unstable;
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
		uint64_t find_conflict(const google::protobuf::RepeatedPtrField<eraftpb::Entry>& ents);

		// maybe_append returns None if the entries cannot be appended. Otherwise,
		// it returns Some(last index of new entries).
		uint64_t maybe_append(uint64_t idx,
				uint64_t term,
				uint64_t committed,
				const google::protobuf::RepeatedPtrField<eraftpb::Entry>& ents);

		uint64_t append(const google::protobuf::RepeatedPtrField<eraftpb::Entry>& ents, int start);
		uint64_t append(std::vector<eraftpb::Entry>& es);

		std::vector<eraftpb::Entry> entries(uint64_t idx, uint64_t max_size);
		std::vector<eraftpb::Entry> all_entries();

		int must_check_outofbounds(uint64_t low, uint64_t high);
		std::vector<eraftpb::Entry> slice(uint64_t low, uint64_t high, uint64_t max_size);

		// is_up_to_date determines if the given (lastIndex,term) log is more up-to-date
		// by comparing the index and term of the last entry in the existing logs.
		// If the logs have last entry with different terms, then the log with the
		// later term is more up-to-date. If the logs end with the same term, then
		// whichever log has the larger last_index is more up-to-date. If the logs are
		// the same, the given log is up-to-date.
		bool is_up_to_date(uint64_t last_index, uint64_t term);

		// next_entries returns all the available entries for execution.
		// If applied is smaller than the index of snapshot, it returns all committed
		// entries after the index of snapshot.
		std::vector<eraftpb::Entry> next_entries();
		//用来判断当前是否有可以写入到Storage中的log
		bool has_next_entries();

		// storage contains all stable entries since the last snapshot.
		Storage* store;

		// unstable contains all unstable entries and snapshot.
		// they will be saved into storage.
		Unstable* unstable;

		// committed is the highest log position that is known to be in
		// stable storage on a quorum of nodes.
		uint64_t committed;

		// applied is the highest log position that the application has
		// been instructed to apply to its state machine.
		// Invariant: applied <= committed
		uint64_t applied;
		std::string tag;
};

#endif
