#ifndef KEYS_H

#include <string>
#include <assert.h>
#include<tuple>
#include <cstring>
#include <stdio.h>

/*
   1、0x01：用于存放StoreIdent，在初始化这个 Store 的时候，我们会将 Store 的 Cluster ID，Store ID 等信息存储到这个 key 里面。
   2、0x02：用来存储 Raft 一些信息，0x02 之后会紧跟该 Raft Region 的 ID（8字节大端序 ），然后在紧跟一个 Suffix 来标识不同的子类型：
   		0x01：用于存放 Raft Log，后面紧跟 Log Index（8字节大端序）
   		0x02：用于存放 RaftLocalState
   		0x03：用于存放 RaftApplyState
   3、0x03：用来存储 Region 本地的一些元信息，0x03 之后紧跟 Raft Region ID，随后在紧跟一个 Suffix 来表示不同的子类型：
   		0x01：用于存放 RegionLocalState
*/
const char MIN_KEY[] = {""};
const char MAX_KEY[] = {0xF, 0xF};

// local is in (0x01, 0x02);
const char LOCAL_PREFIX = 0x01;
const char LOCAL_PREFIX_META = 0x02;
const char LOCAL_PREFIX_REGION = 0x03;
const char LOCAL_PREFIX_IDENT = 0x04;

// Following keys are all local keys, so the first byte must be 0x01.
const char STORE_IDENT_KEY[] = {LOCAL_PREFIX_IDENT, 0x01};

// We save two types region data in DB, for raft and other meta data.
// When the store starts, we should iterate all region meta data to
// construct peer, no need to travel large raft data, so we separate them
// with different prefixes.
const char REGION_RAFT_PREFIX = 0x02;
const char REGION_RAFT_PREFIX_KEY[] = {LOCAL_PREFIX_REGION, REGION_RAFT_PREFIX};

const char REGION_META_PREFIX = 0x03;
const char REGION_META_PREFIX_KEY[] = {LOCAL_PREFIX_META, REGION_META_PREFIX};
const char REGION_META_MIN_KEY[]  ={LOCAL_PREFIX_META, REGION_META_PREFIX, 0};
const char REGION_META_MAX_KEY[] = {LOCAL_PREFIX_META, REGION_META_PREFIX + 1, 0};

// Following are the suffix after the local prefix.
// For region id
const char RAFT_LOG_SUFFIX = 0x01;
const char RAFT_STATE_SUFFIX = 0x02;
const char APPLY_STATE_SUFFIX = 0x03;

// For region meta
const char REGION_STATE_SUFFIX =  0x01;

std::string store_ident_key();

std::string make_region_id_key(uint64_t region_id, char suffix, uint64_t extra_cap);

std::string region_raft_prefix(uint64_t region_id) ;

std::string raft_log_key(uint64_t region_id, uint64_t log_index) ;

std::string raft_state_key(uint64_t region_id) ;

std::string apply_state_key(uint64_t region_id);

/// Get the log index from raft log key generated by `raft_log_key`.
uint64_t raft_log_index(std::string key) ;

/// Get the region id and index from raft log key generated by `raft_log_key`.
std::tuple<uint64_t, uint64_t> decode_raft_log_key(std::string key);

std::string raft_log_prefix(uint64_t region_id) ;
std::string make_region_meta_key(uint64_t region_id, char suffix);
// Decode region meta key, return the region key and meta suffix type.
std::tuple<uint64_t, char> decode_region_meta_key(std::string key) ;

std::string region_meta_prefix(uint64_t region_id);

std::string region_state_key(uint64_t region_id) ;

#endif
