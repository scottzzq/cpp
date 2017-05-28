include ../depends.mk

CXX = g++
ifeq ($(__PERF), 1)
	CXXFLAGS = -O0 -g -pg -pipe -fPIC -D__XDEBUG__ -W -Wwrite-strings -Wpointer-arith -Wreorder -Wswitch -Wsign-promo -Wredundant-decls -Wformat -D_GNU_SOURCE -D__STDC_FORMAT_MACROS -std=c++11 -Wno-unused-variable
else
	CXXFLAGS = -O2 -pipe -fPIC -W -Wwrite-strings -Wpointer-arith -Wreorder -Wswitch -Wsign-promo -Wredundant-decls -Wformat -Wall -D_GNU_SOURCE -D__STDC_FORMAT_MACROS -std=c++11 -Wno-unused-variable -Wno-maybe-uninitialized -Wno-unused-parameter
	# CXXFLAGS = -Wall -W -DDEBUG -g -O0 -D__XDEBUG__ 
endif
OBJECT = tikv_server test_rocksdb

SRC_DIR = ./
PROTOCOL_DIR = ./protocol
RAFT_DIR = ./raft
RAFT_STORE_DIR = ./raftstore
SERVER_DIR = ./server
UTIL_DIR = ./util

OUTPUT = ./output

LIB_PATH = -L../ \
		   -L/home/work/zhaizhiqiang/source/cpp/thirdparty/lib \
		   -L../output/lib/\
		   -L$(MUDO_DIR)/lib/ 

LIBS = -lprotobuf \
			 -pthread \
			 -lmuduo_base_cpp11 \
			 -lmuduo_net_cpp11 \
			 -lslash \
			 -lz \
			 -lrocksdb \
			 -lsnappy \
			 -llz4 \
			 -lbz2 \
			 -lrt

INCLUDE_PATH = -I../output/include/ \
			   -I/home/work/zhaizhiqiang/source/cpp/thirdparty/include \
			   -I/home/work/zhaizhiqiang/source/cpp/thirdparty/include/slash/include \
			   -I$(BOOST_HEADER_DIR)/ \
			   -I$(MUDO_DIR)/include \
			   -I./ \
		   	   -I$(PROTOCOL_DIR) \
		   	   -I$(RAFT_DIR) \
			   -I$(RAFT_STORE_DIR)\
			   -I$(SERVER_DIR) \
			   -I$(UTIL_DIR)


.PHONY: all clean


BASE_BOJS := $(wildcard $(SRC_DIR)/*.cc)
BASE_BOJS += $(wildcard $(SRC_DIR)/*.c)
BASE_BOJS += $(wildcard $(SRC_DIR)/*.cpp)

BASE_BOJS += $(wildcard $(RAFT_DIR)/*.cpp)
BASE_BOJS += $(wildcard $(RAFT_DIR)/*.cc)
BASE_BOJS += $(wildcard $(RAFT_DIR)/*.c)

BASE_BOJS += $(wildcard $(PROTOCOL_DIR)/*.cpp)
BASE_BOJS += $(wildcard $(PROTOCOL_DIR)/*.cc)
BASE_BOJS += $(wildcard $(PROTOCOL_DIR)/*.c)

BASE_BOJS += $(wildcard $(RAFT_STORE_DIR)/*.cpp)
BASE_BOJS += $(wildcard $(RAFT_STORE_DIR)/*.cc)
BASE_BOJS += $(wildcard $(RAFT_STORE_DIR)/*.c)

BASE_BOJS += $(wildcard $(SERVER_DIR)/*.cpp)
BASE_BOJS += $(wildcard $(SERVER_DIR)/*.cc)
BASE_BOJS += $(wildcard $(SERVER_DIR)/*.c)


BASE_BOJS += $(wildcard $(UTIL_DIR)/*.cpp)
BASE_BOJS += $(wildcard $(UTIL_DIR)/*.cc)
BASE_BOJS += $(wildcard $(UTIL_DIR)/*.c)

OBJS = $(patsubst %.cc,%.o,$(BASE_BOJS))

all: $(OBJECT)
	rm -rf $(OUTPUT)
	mkdir -p $(OUTPUT)
	mkdir -p $(OUTPUT)/bin
	mkdir -p $(OUTPUT)/log
	cp $(OBJECT) $(OUTPUT)/bin/
	@echo "Success, go, go, go..."


tikv_server: tikv_server_main.o tikv_common.o raftstore/PeerStorage.o raftstore/Peer.o raftstore/StoreRouter.o raftstore/Store.o raft/Unstable.o raft/Progress.o raft/Storage.o raft/RaftLog.o raft/RawNode.o raft/Raft.o server/TiKVServer.o util/ProtobufCodec.o util/util.o util/keys.o server/StoreClient.o protocol/msgpb.pb.o protocol/coprocessor.pb.o protocol/eraftpb.pb.o protocol/errorpb.pb.o protocol/kvrpcpb.pb.o protocol/metapb.pb.o protocol/msgpb.pb.o protocol/mvccpb.pb.o  protocol/pdpb.pb.o protocol/raft_cmdpb.pb.o protocol/raft_serverpb.pb.o protocol/volume_cmdpb.pb.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(INCLUDE_PATH) $(LIB_PATH) $(LIBS)

test_rocksdb: test_rocksdb.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(INCLUDE_PATH) $(LIB_PATH) $(LIBS)

tikv_client: tikv_client_main.o server/TiKVServer.o util/ProtobufCodec.o server/StoreClient.o protocol/msgpb.pb.o protocol/coprocessor.pb.o protocol/eraftpb.pb.o protocol/errorpb.pb.o protocol/kvrpcpb.pb.o protocol/metapb.pb.o protocol/msgpb.pb.o protocol/mvccpb.pb.o  protocol/pdpb.pb.o protocol/raft_cmdpb.pb.o protocol/raft_serverpb.pb.o protocol/volume_cmdpb.pb.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(INCLUDE_PATH) $(LIB_PATH) $(LIBS)


$(OBJS): %.o : %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE_PATH)  

clean: 
	rm -rf $(OUTPUT)
	rm -rf $(SRC_DIR)/log
	rm -f $(SRC_DIR)/*.o
	rm -f $(PROTOCOL_DIR)/*.o
	rm -f $(RAFT_DIR)/*.o
	rm -f $(RAFT_STORE_DIR)/*.o
	rm -f $(SERVER_DIR)/*.o
	rm -f $(UTIL_DIR)/*.o
	rm -rf $(OBJECT)
