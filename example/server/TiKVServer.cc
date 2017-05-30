#include "TiKVServer.h"
#include "ProtobufCodec.h"
#include "StoreClient.h"
#include "Store.h"
#include "RpcClient.h"
#include "protocol/msgpb.pb.h"

#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
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

		work_thread = new muduo::net::EventLoopThread(boost::bind(&TiKVServer::work_thread_init_func, this));
		work_thread_loop = work_thread->startLoop();

		slash::BaseConf* conf = TiKVConfig::getInstance()->getBaseConf();
		std::string pd_endpoints;
		conf->GetConfStr("pd_endpoints", &pd_endpoints);
		int pd_port;
		conf->GetConfInt("pd_port", &pd_port);

		int cluster_id;
		conf->GetConfInt("cluster_id", &cluster_id);
		pd_client = new RpcClient(pd_endpoints, pd_port, cluster_id);
	}

void TiKVServer::work_thread_init_func(){

}

void TiKVServer::onProtobufMessage(const muduo::net::TcpConnectionPtr& conn,
		const MessagePtr& message,
		muduo::Timestamp receiveTime){
	boost::shared_ptr<msgpb::Message> msg = muduo::down_pointer_cast<msgpb::Message>(message);
	LOG_INFO << "TiKVServer::onProtobufMessage";
	LOG_INFO << "\n" << msg->DebugString();
	switch (msg->msg_type()){
		case msgpb::MessageType::Raft:
			store_router_->on_raft_message(msg->raft());
			break;
		case msgpb::MessageType::Cmd:
			store_router_->on_command_message(msg->cmd_req(), boost::bind(&TiKVServer::response_callback, this, conn, _1));
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

void TiKVServer::resolveAddress(uint64_t store_id, msgpb::Message msg){
	auto resp = this->pd_client->get_store(store_id);
	if (resp.isOk()){
		metapb::Store store = resp.unwrap();	
		std::vector<std::string> strs;
		boost::split(strs, store.address(), boost::is_any_of(":"));
		assert(strs.size() == 2);
		int port = 0;
		std::string host = strs[0];
		sscanf(strs[1].c_str(),"%d",&port); 

		muduo::net::InetAddress serverAddr = muduo::net::InetAddress(host, port);
		StoreClient* client = new StoreClient(store_id, this, loop_, serverAddr);
		client->push_msg(msg);
		client->connect();
		LOG_INFO << "Connecting store_id:[" << store_id << "]" << serverAddr.toIpPort();
	}else{
		LOG_INFO << "resolveAddress, store_id:" << store_id << " failed!";
	}
}

void TiKVServer::sendToStore(uint64_t store_id, msgpb::Message msg){
	auto it = this->store_map_.find(store_id);
	if (it != this->store_map_.end()){
		it->second->send(msg);
		return;
	}

	if (this->store_resolving_set_.count(store_id) > 0){
		// If we are resolving the address, drop the message here.
		LOG_WARN << "store " << store_id << " address is being resolved, drop msg: " << msg.DebugString();
		return;
	}
	work_thread_loop->runInLoop(boost::bind(&TiKVServer::resolveAddress, this, store_id, msg));
	this->store_resolving_set_.insert(store_id);
	LOG_INFO << "insert store_resolving_map_: " << store_id;
	//muduo::net::InetAddress serverAddr;
	//if (store_id == 1){
	//	serverAddr = muduo::net::InetAddress("127.0.0.1", 1234);
	//} else if(store_id == 2){
	//	serverAddr = muduo::net::InetAddress("127.0.0.1", 1235);
	//} else if(store_id == 3){
	//	serverAddr = muduo::net::InetAddress("127.0.0.1", 1236);
	//}

	//StoreClient* client = new StoreClient(store_id, this, loop_, serverAddr);
	//client->connect();
	//LOG_INFO << "Connecting store_id:[" << store_id << "]" << serverAddr.toIpPort();
}

void TiKVServer::StoreConnected(StoreClient* client){
	auto it = this->store_resolving_set_.find(client->getStoreID());
	if (it == this->store_resolving_set_.end()){
		LOG_ERROR << "store_id:[" << client->getStoreID() << "] not in store_resolving_map_";
	}else{
		LOG_INFO << "delete store_id:[" << client->getStoreID() << 
			"] from store_resolving_map_ and insert into store_map_";
		this->store_resolving_set_.erase(it);
	}
	this->store_map_.insert(std::make_pair(client->getStoreID(), client));
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
