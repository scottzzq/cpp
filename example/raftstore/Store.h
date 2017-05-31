#ifndef Store_H
#define Store_H

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>
#include <boost/optional.hpp>
#include "metapb.pb.h"
#include "msgpb.pb.h"

#include "raft_serverpb.pb.h"
#include "raft_cmdpb.pb.h"
#include "result.h"
#include "tikv_common.h"
#include "rocksdb/db.h"

class Peer;
class TiKVServer;
class StoreRouter;

class Store{
	public:
		typedef boost::function<void()> Functor;
		typedef boost::function<void(const raft_cmdpb::RaftCmdResponse&)>  ResponseCallback;

		Store(StoreRouter* router, TiKVServer* server, uint64_t store_id, rocksdb::DB* db_);
		~Store();

		bool init();
		void thread_init_func(muduo::net::EventLoop*);
		void schedule_task();

		inline TiKVServer* get_server(){
			return server;
		}

		void on_raft_message(const raft_serverpb::RaftMessage raft);	
		void on_command_message(const raft_cmdpb::RaftCmdRequest& cmd, ResponseCallback callback);
		void on_pd_heartbeat_tick();
		void heartbeat_pd(Peer* peer);

		void runInLoop(const Functor& cb){
			loop_->runInLoop(cb);
		}
		void add_peer(uint64_t region_id, Peer* p);
		inline uint64_t get_store_id() const{
			return store_id_;
		}
		bool validate_store_id(const raft_cmdpb::RaftCmdRequest& msg);
		bool validate_region(const raft_cmdpb::RaftCmdRequest& msg) ;
		void propose_raft_command(const raft_cmdpb::RaftCmdRequest& msg, 
				ResponseCallback callback);

		void insert_peer_cache(const metapb::Peer& peer);
		boost::optional<metapb::Peer> get_peer_from_cache(uint64_t peer_id);
		Result<bool, Error> check_target_peer_valid(uint64_t region_id, metapb::Peer target);
		inline uint64_t region_count(){
			return region_peers.size();
		}
		bool is_raft_msg_valid(const raft_serverpb::RaftMessage& msg);
	private:
		Store(const Store& s);
		Store& operator=(const Store& s);

		uint64_t store_id_;
		muduo::net::EventLoop* loop_;
		muduo::net::EventLoopThread* thread_;
		// // region_id -> peers
		std::map<uint64_t, Peer* > region_peers;
		std::set<uint64_t> pending_raft_groups;
		// // region end key -> region id
		std::vector<metapb::Region> pending_regions;
		std::map<uint64_t, metapb::Peer> peer_cache;
		std::string tag;
		TiKVServer* server;
		StoreRouter* router;
		rocksdb::DB* db;
};

#endif

