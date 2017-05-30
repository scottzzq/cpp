#ifndef Store_ROUTER_H
#define Store_ROUTER_H

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>
#include "Store.h"
#include "raft_serverpb.pb.h"
#include "raft_cmdpb.pb.h"
#include "metapb.pb.h"
#include "msgpb.pb.h"
#include "rocksdb/db.h"
#include "TiKVConf.h"
#include "PdClient.h"
#include "result.h"
#include "tikv_common.h"
#include <unistd.h>

class TiKVServer;

class StoreRouter{
	public:
		typedef boost::function<void()> Functor;
		typedef boost::function<void(const raft_cmdpb::RaftCmdResponse)>  ResponseCallback;
		StoreRouter(TiKVServer* server, int store_num);
		~StoreRouter();
		bool init();
		void emptyResponse(const raft_cmdpb::RaftCmdResponse);
		void on_raft_message(const raft_serverpb::RaftMessage& raft);	
		void on_command_message(const raft_cmdpb::RaftCmdRequest& cmd, ResponseCallback callback);

		void send_admin_request(metapb::Region region, metapb::Peer peer,
			raft_cmdpb::AdminRequest request);
		void empty_response(const raft_cmdpb::RaftCmdResponse);

		void on_pd_store_heartbeat_tick();
		void handle_store_heartbeat(pdpb::StoreStats stats);
		void handle_heartbeat(metapb::Region region, metapb::Peer peer,
				std::vector<pdpb::PeerStats> down_peers);

		void work_thread_init_func();

		void run_in_work_Loop(const Functor& cb){
			work_thread_loop->runInLoop(cb);
		}

		Result<bool, Error> check_cluster_bootstrapped();
		Result<uint64_t, Error> check_store();
		Result<uint64_t, Error> alloc_id() ;
		Result<uint64_t, Error> bootstrap_store();
		Result<metapb::Region, Error> bootstrap_first_region(uint64_t store_id);
		Result<void, Error> bootstrap_cluster(metapb::Region region);
	private:
		StoreRouter(const StoreRouter& s);
		StoreRouter& operator=(const StoreRouter& s);

		TiKVServer* server_;
		int store_num_;

		muduo::net::EventLoop* work_thread_loop;
		muduo::net::EventLoopThread* work_thread;

		metapb::Store store;
		uint64_t cluster_id;
		rocksdb::DB* db;
		std::vector<Store*> stores_;
		PdClient* pd_client;
};
#endif

