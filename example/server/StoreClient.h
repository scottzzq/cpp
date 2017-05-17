#ifndef STORE_CLIENT_H
#define STORE_CLIENT_H

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpClient.h>
#include <boost/bind.hpp>
#include <utility>
#include <stdio.h>
#include <unistd.h>

#include "ProtobufCodec.h"

class TiKVServer;

class StoreClient : boost::noncopyable {
	public:
		StoreClient(uint64_t store_id, TiKVServer* server, 
				muduo::net::EventLoop* loop, const muduo::net::InetAddress& listenAddr)
			: store_id_(store_id), server_(server), loop_(loop),
			client_(loop, listenAddr, "StoreClient"),
			codec_(boost::bind(&StoreClient::onProtobufMessage, this, _1, _2, _3)) {
				client_.setConnectionCallback(
						boost::bind(&StoreClient::onConnection, this, _1));
				client_.setMessageCallback(
						boost::bind(&StoreClient::onMessage, this, _1, _2, _3));
				client_.enableRetry();
			}
		void onProtobufMessage(const muduo::net::TcpConnectionPtr& conn,
				const MessagePtr& message,
				muduo::Timestamp receiveTime) const{
		}

		void connect() {
			client_.connect();
		}

		uint64_t getStoreID(){
			return store_id_;
		}

		void send(msgpb::Message message){
			codec_.send(this->client_.connection(), message);
		}
	private:
		void onConnection(const muduo::net::TcpConnectionPtr& conn);
		void onMessage(const muduo::net::TcpConnectionPtr& conn, 
				muduo::net::Buffer* buf, muduo::Timestamp receiveTime);
		uint64_t store_id_;
		TiKVServer* server_;
		muduo::net::EventLoop* loop_;
		muduo::net::TcpClient client_;
		ProtobufCodec codec_;
};

#endif
