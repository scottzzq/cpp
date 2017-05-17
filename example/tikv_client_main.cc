#include "StoreClient.h"

#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <boost/bind.hpp>
#include <iostream>
#include <sstream>
#include <stdio.h>

using namespace muduo;
using namespace muduo::net;

int main(int argc, char* argv[])
{
  LOG_INFO << "pid = " << getpid();
    //EventLoop loop;
    //InetAddress serverAddr("127.0.0.1", 1234);
    //StoreClient* client = new StoreClient(&loop, serverAddr);
	//client->connect();
    //loop.loop();
}


