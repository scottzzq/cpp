#ifndef RAFT_STORE_CONFIG_H
#define RAFT_STORE_CONFIG_H

#include <limits.h>
#include <stdint.h>
#include "tikv_common.h"

class Config {
	public:
		static Config getInstance(){
			static Config conf;
			return conf;
		}
		~Config(){
		}

		uint64_t get_capacity(){
			return capacity;
		}
		uint64_t get_raft_base_tick_interval(){
			return raft_base_tick_interval;
		}
		uint64_t get_raft_heartbeat_ticks(){
			return raft_heartbeat_ticks;
		}
		uint64_t get_raft_election_timeout_ticks(){
			return raft_election_timeout_ticks;
		}
		uint64_t get_raft_max_size_per_msg(){
			return raft_max_size_per_msg;
		}
		uint64_t get_raft_max_inflight_msgs(){
			return raft_max_inflight_msgs;
		}
		uint64_t get_raft_entry_max_size(){
			return raft_entry_max_size;
		}
	private:
		Config(){
			capacity = INT_MAX;
			raft_base_tick_interval = RAFT_BASE_TICK_INTERVAL;
            raft_heartbeat_ticks = RAFT_HEARTBEAT_TICKS;
            raft_election_timeout_ticks = RAFT_ELECTION_TIMEOUT_TICKS;
            raft_max_size_per_msg = RAFT_MAX_SIZE_PER_MSG;
            raft_max_inflight_msgs = RAFT_MAX_INFLIGHT_MSGS;
            raft_entry_max_size = RAFT_ENTRY_MAX_SIZE;
		}

		// store capacity.
		// TODO: if not set, we will use disk capacity instead.
		// Now we will use a default capacity if not set.
		uint64_t capacity;

		// raft_base_tick_interval is a base tick interval (ms).
		uint64_t raft_base_tick_interval;
		uint64_t raft_heartbeat_ticks;
		uint64_t raft_election_timeout_ticks;
		uint64_t raft_max_size_per_msg;
		uint64_t raft_max_inflight_msgs;
		// When the entry exceed the max size, reject to propose it.
		uint64_t raft_entry_max_size;

		//// Interval to gc unnecessary raft log (ms).
		//pub raft_log_gc_tick_interval: u64,
		//// A threshold to gc stale raft log, must >= 1.
		//pub raft_log_gc_threshold: u64,
		//// When entry count exceed this value, gc will be forced trigger.
		//pub raft_log_gc_count_limit: u64,
		//// When the approximate size of raft log entries exceed this value,
		//// gc will be forced trigger.
		//pub raft_log_gc_size_limit: u64,

		//// Interval (ms) to check region whether need to be split or not.
		//pub split_region_check_tick_interval: u64,
		///// When region [a, b) size meets region_max_size, it will be split
		///// into two region into [a, c), [c, b). And the size of [a, c) will
		///// be region_split_size (or a little bit smaller).
		//pub region_max_size: u64,
		//pub region_split_size: u64,
		///// When size change of region exceed the diff since last check, it
		///// will be checked again whether it should be split.
		//pub region_check_size_diff: u64,
		///// Interval (ms) to check whether start compaction for a region.
		//pub region_compact_check_interval: u64,
		///// When delete keys of a region exceeds the size, a compaction will
		///// be started.
		//pub region_compact_delete_keys_count: u64,
		//pub pd_heartbeat_tick_interval: u64,
		//pub pd_store_heartbeat_tick_interval: u64,
		//pub snap_mgr_gc_tick_interval: u64,
		//pub snap_gc_timeout: u64,
		//pub lock_cf_compact_interval: u64,
		//pub lock_cf_compact_threshold: u64,

		//pub notify_capacity: usize,
		//pub messages_per_tick: usize,

		///// When a peer is not active for max_peer_down_duration,
		///// the peer is considered to be down and is reported to PD.
		//pub max_peer_down_duration: Duration,

		///// If the leader of a peer is missing for longer than max_leader_missing_duration,
		///// the peer would ask pd to confirm whether it is valid in any region.
		///// If the peer is stale and is not valid in any region, it will destroy itself.
		//pub max_leader_missing_duration: Duration,

		//pub snap_apply_batch_size: usize,

		//// Interval (ms) to check region whether the data is consistent.
		//pub consistency_check_tick_interval: u64,

		//pub report_region_flow_interval: u64,
		//// The lease provided by a successfully proposed and applied entry.
		//pub raft_store_max_leader_lease: TimeDuration,
};
#endif
