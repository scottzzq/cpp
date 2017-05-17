#include "TiKVServer.h"
#include "ProtobufCodec.h"
#include "StoreClient.h"
#include "Store.h"

#include "protocol/msgpb.pb.h"

#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <boost/bind.hpp>
#include <iostream>
#include <sstream>
#include <stdio.h>

TiKVServer::TiKVServer(muduo::net::EventLoop* loop,
		const muduo::net::InetAddress& listenAddr)
: server_(loop, listenAddr, "TikvServer"),
	codec_(boost::bind(&TiKVServer::onProtobufMessage, this, _1, _2, _3)),
	loop_(loop) {
		server_.setConnectionCallback(
				boost::bind(&TiKVServer::onConnection, this, _1));
		server_.setMessageCallback(
				boost::bind(&ProtobufCodec::onMessage, &codec_, _1, _2, _3));
	}

void TiKVServer::onProtobufMessage(const muduo::net::TcpConnectionPtr& conn,
		const MessagePtr& message,
		muduo::Timestamp receiveTime) const {
	boost::shared_ptr<msgpb::Message> msg = muduo::down_pointer_cast<msgpb::Message>(message);
	LOG_INFO << "TiKVServer::onProtobufMessage";
	LOG_INFO << "\n" << msg->DebugString();
	switch (msg->msg_type()){
		case msgpb::MessageType::Raft:
			store_router_->on_raft_message(msg->raft());
			break;
		case msgpb::MessageType::Cmd:
			store_router_->on_command_message(msg->cmd_req());
			break;
		case msgpb::MessageType::KvReq:
		default:
			{
			}
	}
}

void TiKVServer::start() {
	store_router_ = new StoreRouter(this, 1);
	store_router_->init();
	server_.start();
}

void TiKVServer::sendToStore(uint64_t store_id, msgpb::Message msg){
	LOG_INFO << "Send store_id:[" << store_id << "]" << msg.DebugString();
	auto it = this->store_map_.find(store_id);
	if (it != this->store_map_.end()){
		it->second->send(msg);
		return;
	}
	muduo::net::InetAddress serverAddr;
	if (store_id == 4){
		serverAddr = muduo::net::InetAddress("127.0.0.1", 1234);
	} else if(store_id == 5){
		serverAddr = muduo::net::InetAddress("127.0.0.1", 1235);
	} else if(store_id == 6){
		serverAddr = muduo::net::InetAddress("127.0.0.1", 1236);
	}
	StoreClient* client = new StoreClient(store_id, this, loop_, serverAddr);
	client->connect();
	this->store_resolving_map_.insert(std::make_pair(store_id, client));
	LOG_INFO << "Connecting store_id:[" << store_id << "]" << serverAddr.toIpPort();
}

void TiKVServer::StoreConnected(StoreClient* client){
	auto it = this->store_resolving_map_.find(client->getStoreID());
	if (it == this->store_resolving_map_.end()){
		LOG_ERROR << "store_id:[" << client->getStoreID() << "] not in store_resolving_map_";
	}else{
		LOG_INFO << "delete store_id:[" << client->getStoreID() << 
			"] from store_resolving_map_ and insert into store_map_";
		this->store_resolving_map_.erase(it);
		this->store_map_.insert(std::make_pair(client->getStoreID(), client));
	}
}

void TiKVServer::StoreDisConnected(StoreClient* client){
	auto it = this->store_map_.find(client->getStoreID());
	if (it != this->store_map_.end()){
		this->store_map_.erase(it);
	}
}

void TiKVServer::scheduleTask(){
	//sendToStore(1);
	//sendToStore(2);
	//sendToStore(3);
}
