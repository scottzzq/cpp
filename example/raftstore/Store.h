#ifndef Store_H
#define Store_H

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>
#include "metapb.pb.h"

#include "raft_serverpb.pb.h"
#include "raft_cmdpb.pb.h"

class Peer;
class TiKVServer;

class Store{
	public:
		typedef boost::function<void()> Functor;

		Store(TiKVServer* server, uint64_t store_id);
		~Store();

		bool init();

		void threadInitFunc(muduo::net::EventLoop*);
		void scheduleTask();

		TiKVServer* get_server(){
			return server;
		}

		void on_raft_message(const raft_serverpb::RaftMessage raft);	
		void on_command_message(const raft_cmdpb::RaftCmdRequest& cmd);

		void runInLoop(const Functor& cb){
			loop_->runInLoop(cb);
		}

		void add_peer(uint64_t region_id, Peer* p);

		inline uint64_t get_store_id() const{
			return store_id_;
		}
		bool validate_store_id(const raft_cmdpb::RaftCmdRequest& msg);
		bool validate_region(const raft_cmdpb::RaftCmdRequest& msg) ;
		void propose_raft_command(const raft_cmdpb::RaftCmdRequest& msg);
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
};

#endif

