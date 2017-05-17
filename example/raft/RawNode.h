#ifndef RAWNODE_H
#define RAWNODE_H

#include "Raft.h"
#include "eraftpb.pb.h"
#include "metapb.pb.h"
#include "tikv_common.h"
#include "Storage.h"
#include "Config.h"

// Ready encapsulates the entries and messages that are ready to read,
// be saved to stable storage, committed or sent to other peers.
// All fields in Ready are read-only.
class Ready {
	public:
		Ready(Raft* raft);
		~Ready();
		// The current volatile state of a Node.
		// SoftState will be nil if there is no update.
		// It is not required to consume or store SoftState.
		//pub ss: Option<SoftState>,

		// The current state of a Node to be saved to stable storage BEFORE
		// Messages are sent.
		// HardState will be equal to empty state if there is no update.
		//eraftpb::HardState hs;

		// read_states states can be used for node to serve linearizable read requests locally
		// when its applied index is greater than the index in ReadState.
		// Note that the read_state will be returned when raft receives MsgReadIndex.
		// The returned is only valid for the request that requested to read.
		//pub read_states: Vec<ReadState>,

		// Entries specifies entries to be saved to stable storage BEFORE
		// Messages are sent.
		std::vector<eraftpb::Entry> entries;

		// Snapshot specifies the snapshot to be saved to stable storage.
		//pub snapshot: Snapshot,

		// CommittedEntries specifies entries to be committed to a
		// store/state-machine. These have previously been committed to stable
		// store.
		std::vector<eraftpb::Entry> committed_entries;

		// Messages specifies outbound messages to be sent AFTER Entries are
		// committed to stable storage.
		// If it contains a MsgSnap message, the application MUST report back to raft
		// when the snapshot has been received or has failed by calling ReportSnapshot.
		std::vector<eraftpb::Message> messages;
};


class RawNode {
	public:
		RawNode(RaftConfig conf, Storage* storage){
			raft = new Raft(conf, storage);
		}
		~RawNode(){
		}
		inline bool has_ready(){
			if (!raft->msgs.empty()){
				return true;
			}
			return false;
		}

		inline void tick(){
			LOG_INFO << "RawNode::tick";
			raft->tick();
		}

		Ready ready(){
			return Ready(this->raft);
		}

		void step(const eraftpb::Message& m);

		// Advance notifies the RawNode that the application has applied and saved progress in the
		// last Ready results.
		inline void advance(Ready& rd) {
			this->commit_ready(rd);
		}
		// 将已经Commit的RaftLog应用到状态机之后调用，在advance中调用
		void commit_ready(Ready& rd);

		// Campaign causes this RawNode to transition to candidate state.
		inline void campaign() {
			auto m = eraftpb::Message();
			m.set_msg_type(eraftpb::MessageType::MsgHup);
			this->raft->step(m);
		}

		// Propose proposes data be appended to the raft log.
		void propose(std::string data) ;

		// ProposeConfChange proposes a config change.
		void propose_conf_change(eraftpb::ConfChange cc) ;

		eraftpb::ConfState apply_conf_change(eraftpb::ConfChange cc);

		// ReportUnreachable reports the given node is not reachable for the last send.
		void report_unreachable(uint64_t id) ;

		// TransferLeader tries to transfer leadership to the given transferee.
		void transfer_leader(uint64_t transferee) ;

		Raft* raft;
	private:
		SoftState prev_ss;
		eraftpb::HardState prev_hs;
};

#endif

