#include "RawNode.h"
#include "RaftLog.h"

Ready::Ready(Raft* raft){
	this->messages = raft->msgs;
	raft->msgs.clear();
	this->entries = raft->raft_log->unstable_entries();
}

Ready::~Ready(){
}

// Step advances the state machine using the given message.
void RawNode::step(const eraftpb::Message& m){
	// ignore unexpected local messages receiving over network
	//if is_local_msg(m.get_msg_type()) {
	//	return Err(Error::StepLocalMsg);
	//}
	//if (this->raft.prs.contains_key(&m.from()) || !is_response_msg(m.get_msg_type()) {
	LOG_INFO << "RawNode::step: " << m.DebugString();
	this->raft->step(m);
}

void RawNode::commit_ready(Ready& rd) {
	LOG_INFO << "RawNode::commit_ready, size:" << rd.entries.size(); 

	if (!rd.entries.empty()) {
		LOG_INFO << "RawNode::commit_ready:" << rd.entries[0].DebugString(); 

		auto e = rd.entries[rd.entries.size() - 1];
		this->raft->raft_log->stable_to(e.index(), e.term());
	}
}

eraftpb::ConfState RawNode::apply_conf_change(eraftpb::ConfChange cc){
	if (cc.node_id() == INVALID_ID) {
		this->raft->reset_pending_conf();
		auto cs = eraftpb::ConfState();
		for (auto n: this->raft->nodes()){
			cs.add_nodes(n);
		}
		//cs.set_nodes(this->raft->nodes());
		return cs;
	}
	uint64_t nid = cc.node_id();
	assert(cc.has_change_type());
	switch(cc.change_type()) {
		case eraftpb::ConfChangeType::AddNode:
			this->raft->add_node(nid);
			break;
		case eraftpb::ConfChangeType::RemoveNode:
			this->raft->remove_node(nid);
			break;
	}
	auto cs = eraftpb::ConfState();
	for (auto n: this->raft->nodes()){
		cs.add_nodes(n);
	}
	return cs;
}

// Propose proposes data be appended to the raft log.
void RawNode::propose(std::string data) {
	auto m = eraftpb::Message();
	m.set_msg_type(eraftpb::MessageType::MsgPropose);
	m.set_from(this->raft->get_id());
	auto e = m.add_entries();
	e->set_entry_type(eraftpb::EntryType::EntryNormal);
	e->set_data(data);
	this->raft->step(m);
}

// ProposeConfChange proposes a config change.
void RawNode::propose_conf_change(eraftpb::ConfChange cc) {
	//let data = box_try!(protobuf::Message::write_to_bytes(&cc));
	int data_size = cc.ByteSize();
	char* data_buffer = new char[data_size + 1];
	data_buffer[data_size] = '\0';
	cc.SerializeToArray(data_buffer, data_size);

	auto m = eraftpb::Message();
	m.set_msg_type(eraftpb::MessageType::MsgPropose);
	auto e = m.add_entries();
	e->set_entry_type(eraftpb::EntryType::EntryConfChange);
	e->set_data(std::string(data_buffer));
	this->raft->step(m);
	delete []data_buffer;
}
// ReportUnreachable reports the given node is not reachable for the last send.
void RawNode::report_unreachable(uint64_t id) {
	auto m = eraftpb::Message();
	m.set_msg_type(eraftpb::MessageType::MsgUnreachable);
	m.set_from(id);
	// we don't care if it is ok actually
	this->raft->step(m);
}

// TransferLeader tries to transfer leadership to the given transferee.
void RawNode::transfer_leader(uint64_t transferee) {
	auto m = eraftpb::Message();
	m.set_msg_type(eraftpb::MessageType::MsgTransferLeader);
	m.set_from(transferee);
	this->raft->step(m);
}
