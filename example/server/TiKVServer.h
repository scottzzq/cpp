#ifndef TIKV_SERVER_H
#define TIKV_SERVER_H

#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <boost/bind.hpp>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <unordered_map>
#include <unordered_set>

#include "eraftpb.pb.h"
#include "raft_serverpb.pb.h"
#include "msgpb.pb.h"

#include "ProtobufCodec.h"
#include "StoreRouter.h"
#include "PdClient.h"

class StoreClient;
class TiKVServer;

class TiKVServer : boost::noncopyable {
	public:
		typedef boost::function<void()> Functor;
		TiKVServer(muduo::net::EventLoop* loop,
				const muduo::net::InetAddress& listenAddr);
		void work_thread_init_func();
		void onProtobufMessage(const muduo::net::TcpConnectionPtr& conn,
				const MessagePtr& message,
				muduo::Timestamp receiveTime);

		void response_callback(const muduo::net::TcpConnectionPtr conn, const raft_cmdpb::RaftCmdResponse& resp){
			auto resp_msg = msgpb::Message();
			resp_msg.set_msg_type(msgpb::MessageType::CmdResp);
			resp_msg.mutable_cmd_resp()->CopyFrom(resp);
			codec_.send(conn, resp_msg);
		}

		void start();
		void scheduleTask();
		void StoreConnected(StoreClient* client);
		void StoreDisConnected(StoreClient* client);
		void sendToStore(uint64_t store_id, msgpb::Message msg);
		void resolveAddress(uint64_t store_id, msgpb::Message msg);
		void runInLoop(const Functor& cb){
			loop_->runInLoop(cb);
		}
		inline muduo::net::EventLoop* getLoop(){
			return loop_;
		}
		void sendRaftResponse(const muduo::net::TcpConnectionPtr conn, msgpb::Message message);
	private:
		void onConnection(const muduo::net::TcpConnectionPtr& conn) {
			LOG_INFO << conn->localAddress().toIpPort() << " -> "
				<< conn->peerAddress().toIpPort() << " is "
				<< (conn->connected() ? "UP" : "DOWN");
		}
		void onUnknownMessage(const muduo::net::TcpConnectionPtr& conn,
				const MessagePtr& message,
				muduo::Timestamp) {
			LOG_INFO << "onUnknownMessage: " << message->GetTypeName();
			conn->shutdown();
		}

		muduo::net::TcpServer server_;
		ProtobufCodec codec_;
		std::unordered_map<uint64_t, StoreClient* > store_map_;
		std::unordered_set<uint64_t> store_resolving_set_;
		muduo::net::EventLoop* loop_;
		StoreRouter* store_router_;

		muduo::net::EventLoop* work_thread_loop;
		muduo::net::EventLoopThread* work_thread;
		PdClient* pd_client;
};

#endif
