#include "StoreClient.h"
#include "TiKVServer.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpClient.h>

#include <boost/bind.hpp>

#include <utility>

#include <stdio.h>
#include <unistd.h>

void StoreClient::onConnection(const muduo::net::TcpConnectionPtr& conn) {
	if (conn->connected()){
		server_->StoreConnected(this);
	}else{
		server_->StoreDisConnected(this);
	}
	LOG_INFO << conn->localAddress().toIpPort() << " -> "
		<< conn->peerAddress().toIpPort() << " is "
		<< (conn->connected() ? "UP" : "DOWN");
}

void StoreClient::onMessage(const muduo::net::TcpConnectionPtr& conn, 
		muduo::net::Buffer* buf, muduo::Timestamp receiveTime) {
	buf->retrieveAll();
}

void StoreClient::push_msg(msgpb::Message message){
	this->msgs.push_back(message);
}
