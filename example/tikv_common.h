#ifndef TIKV_COMMON_H
#define TIKV_COMMON_H
#include <muduo/base/Logging.h>
#include <stdint.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <set>
#include <map>
#include <vector>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <google/protobuf/message.h>
#include "muduo/net/Callbacks.h"
#include "rocksdb/db.h"
#include "raft_cmdpb.pb.h"

const uint64_t STORE_CAPACITY = INT_MAX;
//Raft Timer
const uint64_t RAFT_BASE_TICK_INTERVAL = 10;

const uint64_t RAFT_HEARTBEAT_TICKS = 10;
const uint64_t RAFT_ELECTION_TIMEOUT_TICKS = 50;

const uint64_t RAFT_MAX_SIZE_PER_MSG = 1024 * 1024;
const uint64_t RAFT_MAX_INFLIGHT_MSGS = 256;
const uint64_t RAFT_ENTRY_MAX_SIZE = 8 * 1024 * 1024;


const int RAFT_LOG_COMPACT_ERROR = 1;
const int RAFT_LOG_INDEX_OUT_OF_BOUND = 2;
const int RAFT_LOG_EMPTY_UNAVABLE= 3;

const int STORAGE_RANGE_LOW_GREAT_HIGH_ERROR = 4;
const int STORAGE_ERROR_UNAVABLE = 5;
const int STORAGE_LOG_ENTRY_OFFSET_ERROR = 6;

const int PTOTOBUG_PARSER_ERROR = 7;

const std::string CF_DEFAULT = "default";
const std::string CF_LOCK = "lock";
const std::string CF_WRITE = "write";
const std::string CF_RAFT = "raft";

// A constant represents invalid id of raft.
const uint64_t INVALID_ID = 0;
const uint64_t NO_LIMIT = INT_MAX;

typedef boost::shared_ptr<google::protobuf::Message> MessagePtr;

google::protobuf::Message* createMessage(const std::string& typeName) ;

void write_initial_state(rocksdb::DB* db, rocksdb::WriteBatch& w, uint64_t region_id);

std::string get_uuid_from_req(const raft_cmdpb::RaftCmdRequest& msg);
#endif
