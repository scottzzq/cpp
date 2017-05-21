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

class TiKVServer;

class StoreRouter{
	public:
		typedef boost::function<void(const raft_cmdpb::RaftCmdResponse)>  ResponseCallback;
		StoreRouter(TiKVServer* server, int store_num);
		~StoreRouter();
		bool init();
		void on_raft_message(const raft_serverpb::RaftMessage& raft);	
		void on_command_message(const raft_cmdpb::RaftCmdRequest& cmd, ResponseCallback callback);
	private:
		StoreRouter(const StoreRouter& s);
		StoreRouter& operator=(const StoreRouter& s);
		metapb::Store store;
 		rocksdb::DB* db;
		TiKVServer* server_;
		int store_num_;
		std::vector<Store*> stores_;
};
#endif

