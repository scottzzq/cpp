#ifndef PeerStorage_H
#define PeerStorage_H

#include "tikv_common.h"
#include "metapb.pb.h"
#include "raft_serverpb.pb.h"
#include "Storage.h"
#include <cassert>
#include "rocksdb/db.h"

class PeerStorage;

struct ApplySnapResult {
	// prev_region is the region before snapshot applied.
	metapb::Region prev_region;
	metapb::Region region;
};

class InvokeContext {
	public:
		InvokeContext(PeerStorage* store);
		void save_raft(uint64_t region_id);
		void save_apply(uint64_t region_id);
		
		rocksdb::WriteBatch wb;
		raft_serverpb::RaftLocalState raft_state;
		raft_serverpb::RaftApplyState apply_state;
		uint64_t last_term;
};

class Ready;

class PeerStorage: public MemStorage{
	public:
		PeerStorage(rocksdb::DB* db, metapb::Region region_);
		virtual ~PeerStorage();
		uint64_t applied_index(){
			return 0;
		}
		metapb::Region get_region();
		RaftState initial_state();
		void handle_raft_ready(Ready& ready);

		virtual uint64_t first_index() const;
		virtual uint64_t last_index() const;
		virtual int term(uint64_t idx, uint64_t& t) const;
		virtual int entries(uint64_t low, uint64_t high, uint64_t max_size,
				std::vector<eraftpb::Entry>& log_entries) const;

		uint64_t truncated_term() const;
		uint64_t truncated_index() const;

		int check_range(uint64_t low, uint64_t high) const;

		inline uint64_t get_region_id() const {
			return this->region.id();
		}
		void append(InvokeContext& ctx, std::vector<eraftpb::Entry>& entries);
		raft_serverpb::RaftLocalState get_raft_state() const{
			return 	this->raft_state;
		}
		raft_serverpb::RaftApplyState get_apply_state() const{
			return this->apply_state;
		}
		uint64_t get_last_term(){
			return last_term;
		}
		rocksdb::DB* get_db(){
			return db;
		}
	private:
		PeerStorage(const PeerStorage& rval);
		PeerStorage& operator =(const PeerStorage& rval);

		rocksdb::DB* db;
		metapb::Region region;
		raft_serverpb::RaftLocalState raft_state;
		raft_serverpb::RaftApplyState apply_state;
		uint64_t applied_index_term;
		uint64_t last_term;
};

#endif
