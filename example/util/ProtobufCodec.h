// Copyright 2011, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_EXAMPLES_PROTOBUF_CODEC_CODEC_H
#define MUDUO_EXAMPLES_PROTOBUF_CODEC_CODEC_H

#include <muduo/net/Buffer.h>
#include <muduo/net/TcpConnection.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <muduo/base/Logging.h>
#include <google/protobuf/message.h>
#include "msgpb.pb.h"


// | 0xdaf4(2 bytes magic value) | 0x01(version 2 bytes) | msg_len(4 bytes) | msg_id(8 bytes) |
// struct ProtobufTransportFormat __attribute__ ((__packed__))
// {
//   int16_t  magic_num;
//   int16_t  version;
//   int32_t  msg_len;
//   int64_t  msg_id;
//   char     protobufData[msg_len];
// }
typedef boost::shared_ptr<google::protobuf::Message> MessagePtr;

class ProtobufCodec : boost::noncopyable {
	public:
		enum ErrorCode {
			kNoError = 0,
			kInvalidLength,
			kCheckSumError,
			kInvalidNameLen,
			kUnknownMessageType,
			kParseError,
		};
		typedef boost::function<void (const muduo::net::TcpConnectionPtr&,
				const MessagePtr&,
				muduo::Timestamp)> ProtobufMessageCallback;
		typedef boost::function<void (const muduo::net::TcpConnectionPtr&,
				muduo::net::Buffer*,
				muduo::Timestamp,
				ErrorCode)> ErrorCallback;
		explicit ProtobufCodec(const ProtobufMessageCallback& messageCb)
			: messageCallback_(messageCb),
			errorCallback_(defaultErrorCallback) {
			}

		ProtobufCodec(const ProtobufMessageCallback& messageCb, const ErrorCallback& errorCb)
			: messageCallback_(messageCb),
			errorCallback_(errorCb) {
			}

		void onMessage(const muduo::net::TcpConnectionPtr& conn,
				muduo::net::Buffer* buf,
				muduo::Timestamp receiveTime);

		void send(const muduo::net::TcpConnectionPtr& conn,
				msgpb::Message message) {
			muduo::net::Buffer buf;
			fillEmptyBuffer(&buf, message);
			LOG_INFO << "conn:" << conn.get() << " send bytes size:" << buf.readableBytes() << " ProtobufCodec::send " << message.DebugString();
			conn->send(&buf);
		}
		static const muduo::string& errorCodeToString(ErrorCode errorCode);
		static void fillEmptyBuffer(muduo::net::Buffer* buf, msgpb::Message message);
		//static google::protobuf::Message* createMessage(const std::string& type_name);
		static MessagePtr parse(const char* buf, int len, ErrorCode* errorCode);
	private:
		static void defaultErrorCallback(const muduo::net::TcpConnectionPtr&,
				muduo::net::Buffer*,
				muduo::Timestamp,
				ErrorCode);
		ProtobufMessageCallback messageCallback_;
		ErrorCallback errorCallback_;
		const static int kHeaderLen = sizeof(char) * 16;
		const static int kMinMessageLen = kHeaderLen; 
		const static int kMaxMessageLen = 1024*1024*1024; 
};

#endif  // MUDUO_EXAMPLES_PROTOBUF_CODEC_CODEC_H
