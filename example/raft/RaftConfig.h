#ifndef RAFT_CONFIG_H
#define RAFT_CONFIG_H

#include <stdint.h>
#include <string>
#include "TiKVConf.h"

class RaftConfig {
	public:
		RaftConfig(){
			//develop
			slash::BaseConf* conf = TiKVConfig::getInstance()->getBaseConf();
			int store_id;
			conf->GetConfInt("store_id", &store_id);
			id = store_id;

		}
		~RaftConfig(){
		}
		/// id is the identity of the local raft. ID cannot be 0.
		uint64_t id;

		/// peers contains the IDs of all nodes (including self) in
		/// the raft cluster. It should only be set when starting a new
		/// raft cluster.
		/// Restarting raft from previous configuration will panic if
		/// peers is set.
		/// peer is private and only used for testing right now.
		std::vector<uint64_t> peers;

		/// ElectionTick is the number of node.tick invocations that must pass between
		/// elections. That is, if a follower does not receive any message from the
		/// leader of current term before ElectionTick has elapsed, it will become
		/// candidate and start an election. election_tick must be greater than
		/// HeartbeatTick. We suggest election_tick = 10 * HeartbeatTick to avoid
		/// unnecessary leader switching
		uint64_t election_tick;
		/// HeartbeatTick is the number of node.tick invocations that must pass between
		/// heartbeats. That is, a leader sends heartbeat messages to maintain its
		/// leadership every heartbeat ticks.
		uint64_t heartbeat_tick;

		/// Applied is the last applied index. It should only be set when restarting
		/// raft. raft will not return entries to the application smaller or equal to Applied.
		/// If Applied is unset when restarting, raft might return previous applied entries.
		/// This is a very application dependent configuration.
		uint64_t applied;

		/// MaxSizePerMsg limits the max size of each append message. Smaller value lowers
		/// the raft recovery cost(initial probing and message lost during normal operation).
		/// On the other side, it might affect the throughput during normal replication.
		/// Note: math.MaxUusize64 for unlimited, 0 for at most one entry per message.
		uint64_t max_size_per_msg;
		/// max_inflight_msgs limits the max number of in-flight append messages during optimistic
		/// replication phase. The application transportation layer usually has its own sending
		/// buffer over TCP/UDP. Setting MaxInflightMsgs to avoid overflowing that sending buffer.
		/// TODO: feedback to application to limit the proposal rate?
		uint64_t max_inflight_msgs;

		/// check_quorum specifies if the leader should check quorum activity. Leader steps down when
		/// quorum is not active for an electionTimeout.
		bool check_quorum;

		/// pre_vote enables the Pre-Vote algorithm described in raft thesis section
		/// 9.6. This prevents disruption when a node that has been partitioned away
		/// rejoins the cluster.
		bool pre_vote;

		/// read_only_option specifies how the read only request is processed.
		//pub read_only_option: ReadOnlyOption,

		/// tag is only used for logging
		std::string tag;
};
#endif
