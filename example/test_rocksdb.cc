#include "rocksdb/db.h"
#include <iostream>  
#include <stdlib.h>  
#include <thread>  
#include <string>  
#include "env.h"
#include "result.h"
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
#include "keys.h"

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

void my_thread(rocksdb::DB* db) {  
	uint64_t start = slash::NowMicros();
	auto total = 250000;
	for (int i = 0 ; i < total; ++i){
		rocksdb::WriteBatch wb;
		wb.Put("testA", "testB");
		auto s = db->Write(rocksdb::WriteOptions(), &wb);
		assert(s.ok());
	}
	uint64_t end = slash::NowMicros();
	std::cout << ((end - start) / 1000) << " ms"<< std::endl;
}  

int main(int argc, char* argv[]){
	srand(time(NULL));

	char buffer[10];
	g_file = fopen("./tikv_server.log", "w");
	setbuffer(g_file, buffer, sizeof buffer);
	muduo::Logger::setOutput(dummyOutput);


	rocksdb::DB* db;
	rocksdb::Options options;
	options.create_if_missing = true;
	rocksdb::Status status = rocksdb::DB::Open(options, argv[1], &db);
	assert(status.ok());

	rocksdb::Iterator* it = db->NewIterator(rocksdb::ReadOptions());
	it->Seek("");
	for (; it->Valid(); it->Next()){
		LOG_INFO << "key:" << std::string(it->key().data(), it->key().size()) << "value:" << std::string(it->value().data(), it->value().size());
	}

	//std::vector<std::thread* > threads;
	//auto thread_num = 1;
	//for (int i = 0; i < thread_num; ++i){
	//	std::thread* t = new std::thread(my_thread, db);  
	//	threads.push_back(t);
	//}
	//for (int i = 0; i < thread_num; ++i){
	//	threads[i]->join();
	//}

	//Result<uint32_t, uint32_t> r1 = Err(3u);
	//if (r1.isOk())	{
	//	uint32_t re = r1.unwrap();
	//	std::cout << "Ok:" << re << std::endl;
	//}else{
	//	uint32_t err = r1.unwrapErr();
	//	std::cout << "Error:" << err << std::endl;
	//}
	return 0;	
}
