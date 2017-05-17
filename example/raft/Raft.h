#ifndef RAFT_H
#define RAFT_H

#include <stdint.h>
#include <map>
#include <vector>
#include <string>
#include "eraftpb.pb.h"
#include "tikv_common.h"
#include "eraftpb.pb.h"
#include "RaftConfig.h"

enum StateRole {
	Follower,
	Candidate,
	Leader,
	PreCandidate,
};

struct SoftState {
	uint64_t leader_id;
	StateRole raft_state;
};


class Storage;
class Progress;
class RaftLog;

class Raft{
	public:
		Raft(RaftConfig conf, Storage* storage);
		~Raft();

		inline const Storage* get_store(){
			return storage_;
		}

		inline Storage* mut_store() {
			return storage_;
		}

		SoftState soft_state();
		eraftpb::HardState hard_state();

		inline bool in_lease()  {
			return this->state == StateRole::Leader && this->check_quorum;
		}

		uint64_t quorum() {
			return this->prs.size() / 2 + 1;
		}

		// for testing leader lease
		inline void set_randomized_election_timeout(uint64_t t) {
			this->randomized_election_timeout = t;
		}

		inline uint64_t get_election_timeout(){
			return election_timeout;
		}

		inline uint64_t get_heartbeat_timeout() {
			return this->heartbeat_timeout;
		}

		inline uint64_t get_randomized_election_timeout() {
			return randomized_election_timeout;
		}

		inline std::vector<uint64_t> nodes() {
			std::vector<uint64_t> nodes;
			std::map<uint64_t, Progress*>::const_iterator it = this->prs.begin();
			while (it != this->prs.end()){
				nodes.push_back(it->first);
				++it;
			}
			std::sort(nodes.begin(), nodes.end());
			return nodes;
		}

		void send(eraftpb::Message& m);
		void prepare_send_entries(eraftpb::Message& m, uint64_t to, uint64_t term, 
				std::vector<eraftpb::Entry>& ents);
		void send_append(uint64_t to);
		void send_heartbeat(uint64_t to, std::string ctx);

		void bcast_append();
		void bcast_heartbeat();
		void bcast_heartbeat_with_ctx(std::string ctx);

		bool maybe_commit();
		void reset(uint64_t term);
		void append_entry(std::vector<eraftpb::Entry>& es);

		void tick();
		void tick_election();
		void tick_heartbeat();

		void become_follower(uint64_t term, uint64_t leader_id);
		void become_candidate();
		void become_leader();
		void become_pre_candidate();


		uint64_t num_pending_conf(std::vector<eraftpb::Entry>& ents);
		void campaign(std::string campaign_type);
		uint64_t poll(uint64_t id, bool v);

		void step(eraftpb::Message msg);
		void step_candidate(eraftpb::Message msg);
		void step_follower(eraftpb::Message& msg);
		void step_leader(eraftpb::Message msg);


		void handle_append_entries(eraftpb::Message& m) ;
		void handle_heartbeat(eraftpb::Message m);

		inline bool promotable(){
			return this->prs.count(this->id) == 1;
		}
		void add_node(uint64_t id);
		void remove_node(uint64_t id);

		void reset_pending_conf() {
			this->pending_conf = false;
		}

		inline void set_progress(uint64_t node_id, uint64_t matched, uint64_t next_idx) ;
		inline void del_progress(uint64_t node_id) ;
		void load_state(eraftpb::HardState hs) ;

		bool pass_election_timeout();
		void reset_randomized_election_timeout();
		bool check_quorum_active();

		inline void abort_leader_transfer() {
			this->lead_transferee = INVALID_ID;
		}
		void send_timeout_now(uint64_t to);

		inline uint64_t get_id() const{
			return this->id;
		}

		inline StateRole get_state(){
			return state;
		}

		inline uint64_t get_term(){
			return this->term;
		}
		/// lead_transferee is id of the leader transfer target when its value is not None.
		/// Follow the procedure defined in raft thesis 3.10.
		uint64_t lead_transferee;


		std::vector<eraftpb::Message> msgs;
		RaftLog* raft_log;
	private:
		void log_vote_approve(eraftpb::Message& m);
		void log_vote_reject(eraftpb::Message& m);
		void handle_append_response(eraftpb::Message& m,
				bool& old_paused,
				bool& send_append,
				bool& maybe_commit);
		void handle_transfer_leader(eraftpb::Message& m);
		/// check message's progress to decide which action should be taken.
		void check_message_with_progress(eraftpb::Message& m,
				bool& send_append,
				bool& old_paused,
				bool& maybe_commit,
				eraftpb::Message& more_to_send);

	private:
		uint64_t id;
		uint64_t leader_id;

		StateRole state;

		uint64_t term;
		uint64_t vote;

		std::map<uint64_t, bool> votes;

		bool pending_conf;
		uint64_t max_inflight;
		uint64_t max_msg_size;
		std::map<uint64_t, Progress*> prs;

		bool check_quorum;
		bool pre_vote;

		uint64_t election_elapsed;

		/// number of ticks since it reached last heartbeatTimeout.
		/// only leader keeps heartbeatElapsed.
		//leader用来定期给Follower发送心跳包，标识tick时间
		uint64_t heartbeat_elapsed;

		// [election_timeout, 2 * election_timeout - 1]. It gets reset
		// when raft changes its state to follower or candidate.
		uint64_t randomized_election_timeout;
		//心跳超时
		uint64_t heartbeat_timeout;
		//选举超时
		uint64_t election_timeout;
		/// tag is only used for logging
		std::string tag;
		Storage* storage_;
};
#endif
