#include "tikv_common.h"

google::protobuf::Message* createMessage(const std::string& typeName) {
	google::protobuf::Message* message = NULL;
	const google::protobuf::Descriptor* descriptor =
		google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);
	if (descriptor) {
		const google::protobuf::Message* prototype =
			google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype) {
			message = prototype->New();
		}
	}
	return message;
}

std::string get_uuid_from_req(const raft_cmdpb::RaftCmdRequest& msg){
	return msg.header().uuid();
}

