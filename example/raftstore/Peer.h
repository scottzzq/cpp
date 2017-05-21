#ifndef PEER_H
#define PEER_H

#include <stdint.h>
#include <string>
#include <deque>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <boost/optional.hpp>
#include "rocksdb/db.h"

#include "metapb.pb.h"
#include "RawNode.h"
#include "raft_cmdpb.pb.h"
#include "raft_serverpb.pb.h"
#include "raft_cmdpb.pb.h"
#include "eraftpb.pb.h"
#include "Config.h"
#include "metapb.pb.h"
#include "rocksdb/db.h"

typedef boost::function<void(raft_cmdpb::RaftCmdResponse message)> ResponseCallback;

struct ChangePeer{
	eraftpb::ConfChangeType change_type;
	metapb::Peer peer;
	metapb::Region region;
};

struct CompactLog{
	raft_serverpb::RaftTruncatedState state;
};

struct SplitRegion{
	metapb::Region left;
	metapb::Region right;
};

struct ExecResult {
	boost::optional<ChangePeer> change_peer;
	boost::optional<CompactLog> compact_log;
	boost::optional<SplitRegion> split_region;
};

class ExecContext {
	public:
		ExecContext(raft_serverpb::RaftApplyState apply_state, 
				raft_cmdpb::RaftCmdRequest& req);
		~ExecContext();
		void save(uint64_t region_id);
		raft_serverpb::RaftApplyState apply_state;
		rocksdb::WriteBatch wb;
		raft_cmdpb::RaftCmdRequest req;
};

struct PendingCmd {
		std::string uuid;
		uint64_t term;
		ResponseCallback cb;
};

class PendingCmdQueue {
	public:
		PendingCmdQueue();
		~PendingCmdQueue();
		bool contains(std::string uuid) const ;
		void remove(boost::optional<PendingCmd> cmd) ;
		boost::optional<PendingCmd> pop_normal(uint64_t term);
		void append_normal(PendingCmd& cmd) ;
		boost::optional<PendingCmd> take_conf_change();
		void set_conf_change(PendingCmd cmd) ;

		std::deque<PendingCmd> normals;
		boost::optional<PendingCmd> conf_change;
		std::unordered_set<std::string> uuids;
};

class PeerStorage;
class Store;

class Peer{
	public:
		Peer(rocksdb::DB* db, Store* store_, metapb::Region r);
		~Peer();
		// If we create the peer actively, like bootstrap/split/merge region, we should
		// use this function to create the peer. The region must contain the peer info
		// for this store.
		static Peer* create();

		// The peer can be created from another node with raft membership changes, and we only
		// know the region_id and peer_id when creating this replicated peer, the region info
		// will be retrieved later after applying snapshot.
		// 有peer_id和region_id就可以创建出Peer来
		static Peer* replicate();

		/// Propose a request.
		///
		/// Return true means the request has been proposed successfully.
		bool propose(PendingCmd cmd, raft_cmdpb::RaftCmdRequest req, raft_cmdpb::RaftCmdResponse& resp);

		void handle_raft_ready();
		std::vector<ExecResult> handle_raft_commit_entries(std::vector<eraftpb::Entry>& committed_entries);
		boost::optional<ExecResult> handle_raft_entry_normal(eraftpb::Entry& entry);
		boost::optional<ExecResult> handle_raft_entry_conf_change(eraftpb::Entry& entry);
	
		void send(std::vector<eraftpb::Message>& msg);
		void send_raft_message(eraftpb::Message& msg);
		RawNode* raft_group;
		inline boost::optional<metapb::Peer> get_peer_from_cache(uint64_t peer_id){
			auto it = this->peer_cache.find(peer_id);
			if (it != this->peer_cache.end()){
				return it->second;
			}
			return boost::optional<metapb::Peer>(boost::none);
		}

		metapb::Region region();

		PeerStorage* get_store();
		void step(const eraftpb::Message& m);

		bool is_leader();
		uint64_t term();

		inline uint64_t peer_id() {
			return this->peer.id();
		}

		bool check_epoch(const raft_cmdpb::RaftCmdRequest& req);

		bool is_local_read(raft_cmdpb::RaftCmdRequest& req) ;
		bool propose_normal(raft_cmdpb::RaftCmdRequest cmd);
		uint64_t next_proposal_index();
		void process_raft_cmd(uint64_t index, uint64_t term,
				raft_cmdpb::RaftCmdRequest& cmd);

		raft_cmdpb::RaftCmdResponse exec_raft_cmd(ExecContext& ctx);
		raft_cmdpb::RaftCmdResponse exec_write_cmd(ExecContext& ctx);
		raft_cmdpb::Response do_put(ExecContext& ctx, raft_cmdpb::Request& req);
		raft_cmdpb::RaftCmdResponse apply_raft_cmd(uint64_t index, uint64_t term,
				raft_cmdpb::RaftCmdRequest& req);
		uint64_t leader_id();
		void notify_not_leader(const PendingCmd& cmd);

		boost::optional<ResponseCallback> find_cb(std::string uuid, uint64_t term, 
				const raft_cmdpb::RaftCmdRequest& cmd);
	private:
			Peer& operator= (const Peer& p);
		Peer(const Peer& p);
		rocksdb::DB* db;
		PeerStorage* peer_storage;
		std::map<uint64_t, metapb::Peer> peer_cache;
		metapb::Peer peer;
		uint64_t region_id;
		//proposals: ProposalQueue,
		PendingCmdQueue pending_cmds;
		// // Record the last instant of each peer's heartbeat response.
		std::unordered_map<uint64_t, uint64_t> peer_heartbeats;
		// /// an inaccurate difference in region size since last reset.
		std::string tag;
		uint64_t last_compacted_idx;
		// // Approximate size of logs that is applied but not compacted yet.
		// pub raft_log_size_hint: u64,
		// // When entry exceed max size, reject to propose the entry.
		uint64_t raft_entry_max_size;

		// // if we remove ourself in ChangePeer remove, we should set this flag, then
		// // any following committed logs in same Ready should be applied failed.
		bool pending_remove;
		uint64_t leader_missing_time;
		uint64_t leader_lease_expired_time;
		uint64_t election_timeout;
		Store* store;
};

#endif
