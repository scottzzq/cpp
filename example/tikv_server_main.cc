#include "TiKVServer.h"
#include "TiKVConf.h"
#include "tikv_common.h"

#include <muduo/base/Logging.h>
#include <muduo/base/LogFile.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include "protocol/msgpb.pb.h"
#include <boost/bind.hpp>
#include <iostream>
#include <sstream>
#include <stdio.h>

int g_total;
FILE* g_file;
boost::scoped_ptr<muduo::LogFile> g_logFile;

void dummyOutput(const char* msg, int len) {
	g_total += len;
	if (g_file) {
		fwrite(msg, 1, len, g_file);
	} else if (g_logFile) {
		g_logFile->append(msg, len);
	}
}

int main(int argc, char* argv[]) {
	srand(time(NULL));

	char buffer[10];
	g_file = fopen("./tikv_server.log", "w");
	setbuffer(g_file, buffer, sizeof buffer);
	muduo::Logger::setOutput(dummyOutput);

	LOG_INFO << "pid = " << getpid();
	if (argc > 1) {
		slash::BaseConf* conf = TiKVConfig::getInstance()->getBaseConf();
		int port;
		conf->GetConfInt("port", &port);
		LOG_INFO << "port:" << port;

		muduo::net::EventLoop loop;
		muduo::net::InetAddress serverAddr(port);
		TiKVServer server(&loop, serverAddr);
		server.start();
		loop.loop();
	} else {
		printf("Usage: %s port\n", argv[0]);
	}
}


