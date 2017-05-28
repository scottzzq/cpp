// Copyright 2011, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "ProtobufCodec.h"

#include <muduo/base/Logging.h>
#include <muduo/net/Endian.h>
#include "google-inl.h"
#include "msgpb.pb.h"

#include <google/protobuf/descriptor.h>

#include <zlib.h>  // adler32
#include "tikv_common.h"

using namespace muduo;
using namespace muduo::net;

void ProtobufCodec::fillEmptyBuffer(Buffer* buf, msgpb::Message message) {
	assert(buf->readableBytes() == 0);
	int byte_size = message.ByteSize();
	buf->appendInt16(0xdaf4);
	buf->appendInt16(0x01);
	buf->appendInt32(byte_size);
	buf->appendInt64(0x01);

	buf->ensureWritableBytes(byte_size);

	uint8_t* start = reinterpret_cast<uint8_t*>(buf->beginWrite());
	uint8_t* end = message.SerializeWithCachedSizesToArray(start);
	if (end - start != byte_size) {
		ByteSizeConsistencyError(byte_size, message.ByteSize(), static_cast<int>(end - start));
	}
	buf->hasWritten(byte_size);
}

namespace {
	const string kNoErrorStr = "NoError";
	const string kInvalidLengthStr = "InvalidLength";
	const string kCheckSumErrorStr = "CheckSumError";
	const string kInvalidNameLenStr = "InvalidNameLen";
	const string kUnknownMessageTypeStr = "UnknownMessageType";
	const string kParseErrorStr = "ParseError";
	const string kUnknownErrorStr = "UnknownError";
}

const string& ProtobufCodec::errorCodeToString(ErrorCode errorCode) {
	switch (errorCode) {
		case kNoError:
			return kNoErrorStr;
		case kInvalidLength:
			return kInvalidLengthStr;
		case kCheckSumError:
			return kCheckSumErrorStr;
		case kInvalidNameLen:
			return kInvalidNameLenStr;
		case kUnknownMessageType:
			return kUnknownMessageTypeStr;
		case kParseError:
			return kParseErrorStr;
		default:
			return kUnknownErrorStr;
	}
}

void ProtobufCodec::defaultErrorCallback(const muduo::net::TcpConnectionPtr& conn,
		muduo::net::Buffer* buf,
		muduo::Timestamp,
		ErrorCode errorCode) {
	LOG_ERROR << "ProtobufCodec::defaultErrorCallback - " << errorCodeToString(errorCode);
	if (conn && conn->connected()) {
		conn->shutdown();
	}
}

int32_t asInt32(const char* buf) {
	int32_t be32 = 0;
	::memcpy(&be32, buf, sizeof(be32));
	return sockets::networkToHost32(be32);
}

void ProtobufCodec::onMessage(const TcpConnectionPtr& conn,
		Buffer* buf,
		Timestamp receiveTime) {

	//LOG_INFO << conn.get() << ": total size:" << buf->readableBytes();
	while (buf->readableBytes() >= kMinMessageLen) {
		int16_t be16 = 0;
		int32_t be32 = 0;
		int64_t be64 = 0;
		::memcpy(&be16, buf->peek(), sizeof be16);
		be16 = sockets::networkToHost16(be16);
		const uint16_t magic_num = be16;

		::memcpy(&be16, buf->peek() + 2, sizeof be16);
		be16 = sockets::networkToHost16(be16);
		const uint16_t version = be16;

		::memcpy(&be32, buf->peek() + 2 + 2, sizeof be32);
		be32 = sockets::networkToHost32(be32);
		const uint32_t msg_len = be32;

		::memcpy(&be64, buf->peek() + 2 + 2 + 4, sizeof be64);
		be64 = sockets::networkToHost64(be64);
		const uint64_t msg_id = be64;

		//LOG_INFO << conn.get() << ": readableBytes size:" << buf->readableBytes() << " msg_len:" << msg_len;
		if (msg_len > kMaxMessageLen) {
			errorCallback_(conn, buf, receiveTime, kInvalidLength);
			break;
		} else if (buf->readableBytes() >= implicit_cast<size_t>(msg_len + kHeaderLen)) {
			ErrorCode errorCode = kNoError;
			MessagePtr message = parse(buf->peek() + kHeaderLen, msg_len, &errorCode);
			if (errorCode == kNoError && message) {
				messageCallback_(conn, message, receiveTime);
				buf->retrieve(msg_len + 16);
			} else {
				errorCallback_(conn, buf, receiveTime, errorCode);
				break;
			}
		} else {
			break;
		}
	}
}

//google::protobuf::Message* ProtobufCodec::createMessage(const std::string& typeName) {
//	google::protobuf::Message* message = NULL;
//	const google::protobuf::Descriptor* descriptor =
//		google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);
//	if (descriptor) {
//		const google::protobuf::Message* prototype =
//			google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
//		if (prototype) {
//			message = prototype->New();
//		}
//	}
//	return message;
//}

MessagePtr ProtobufCodec::parse(const char* buf, int len, ErrorCode* error) {
	typedef msgpb::Message T;
	std::string typeName = T::descriptor()->full_name();

	MessagePtr message;
	message.reset(createMessage(typeName));
	if (message) {
		const char* data = buf;
		int32_t dataLen = len;
		if (message->ParseFromArray(data, dataLen)) {
			*error = kNoError;
		} else {
			*error = kParseError;
		}
	} else {
		*error = kUnknownMessageType;
	}
	return message;
}

