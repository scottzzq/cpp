// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: volume_cmdpb.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "volume_cmdpb.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace volume_cmdpb {

namespace {

const ::google::protobuf::Descriptor* AddRequest_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  AddRequest_reflection_ = NULL;
const ::google::protobuf::Descriptor* AddResponse_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  AddResponse_reflection_ = NULL;
const ::google::protobuf::Descriptor* Request_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Request_reflection_ = NULL;
const ::google::protobuf::Descriptor* Response_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Response_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* CmdType_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_volume_5fcmdpb_2eproto() {
  protobuf_AddDesc_volume_5fcmdpb_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "volume_cmdpb.proto");
  GOOGLE_CHECK(file != NULL);
  AddRequest_descriptor_ = file->message_type(0);
  static const int AddRequest_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AddRequest, count_),
  };
  AddRequest_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      AddRequest_descriptor_,
      AddRequest::default_instance_,
      AddRequest_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AddRequest, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AddRequest, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(AddRequest));
  AddResponse_descriptor_ = file->message_type(1);
  static const int AddResponse_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AddResponse, volume_ids_),
  };
  AddResponse_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      AddResponse_descriptor_,
      AddResponse::default_instance_,
      AddResponse_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AddResponse, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AddResponse, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(AddResponse));
  Request_descriptor_ = file->message_type(2);
  static const int Request_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Request, cmd_type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Request, add_),
  };
  Request_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Request_descriptor_,
      Request::default_instance_,
      Request_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Request, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Request, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Request));
  Response_descriptor_ = file->message_type(3);
  static const int Response_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Response, cmd_type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Response, add_),
  };
  Response_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Response_descriptor_,
      Response::default_instance_,
      Response_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Response, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Response, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Response));
  CmdType_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_volume_5fcmdpb_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    AddRequest_descriptor_, &AddRequest::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    AddResponse_descriptor_, &AddResponse::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Request_descriptor_, &Request::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Response_descriptor_, &Response::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_volume_5fcmdpb_2eproto() {
  delete AddRequest::default_instance_;
  delete AddRequest_reflection_;
  delete AddResponse::default_instance_;
  delete AddResponse_reflection_;
  delete Request::default_instance_;
  delete Request_reflection_;
  delete Response::default_instance_;
  delete Response_reflection_;
}

void protobuf_AddDesc_volume_5fcmdpb_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\022volume_cmdpb.proto\022\014volume_cmdpb\"\033\n\nAd"
    "dRequest\022\r\n\005count\030\001 \001(\004\"!\n\013AddResponse\022\022"
    "\n\nvolume_ids\030\001 \003(\004\"Y\n\007Request\022\'\n\010cmd_typ"
    "e\030\001 \001(\0162\025.volume_cmdpb.CmdType\022%\n\003add\030\002 "
    "\001(\0132\030.volume_cmdpb.AddRequest\"[\n\010Respons"
    "e\022\'\n\010cmd_type\030\001 \001(\0162\025.volume_cmdpb.CmdTy"
    "pe\022&\n\003add\030\002 \001(\0132\031.volume_cmdpb.AddRespon"
    "se*\037\n\007CmdType\022\013\n\007Invalid\020\000\022\007\n\003Add\020\001", 315);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "volume_cmdpb.proto", &protobuf_RegisterTypes);
  AddRequest::default_instance_ = new AddRequest();
  AddResponse::default_instance_ = new AddResponse();
  Request::default_instance_ = new Request();
  Response::default_instance_ = new Response();
  AddRequest::default_instance_->InitAsDefaultInstance();
  AddResponse::default_instance_->InitAsDefaultInstance();
  Request::default_instance_->InitAsDefaultInstance();
  Response::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_volume_5fcmdpb_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_volume_5fcmdpb_2eproto {
  StaticDescriptorInitializer_volume_5fcmdpb_2eproto() {
    protobuf_AddDesc_volume_5fcmdpb_2eproto();
  }
} static_descriptor_initializer_volume_5fcmdpb_2eproto_;
const ::google::protobuf::EnumDescriptor* CmdType_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CmdType_descriptor_;
}
bool CmdType_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}


// ===================================================================

#ifndef _MSC_VER
const int AddRequest::kCountFieldNumber;
#endif  // !_MSC_VER

AddRequest::AddRequest()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:volume_cmdpb.AddRequest)
}

void AddRequest::InitAsDefaultInstance() {
}

AddRequest::AddRequest(const AddRequest& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:volume_cmdpb.AddRequest)
}

void AddRequest::SharedCtor() {
  _cached_size_ = 0;
  count_ = GOOGLE_ULONGLONG(0);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

AddRequest::~AddRequest() {
  // @@protoc_insertion_point(destructor:volume_cmdpb.AddRequest)
  SharedDtor();
}

void AddRequest::SharedDtor() {
  if (this != default_instance_) {
  }
}

void AddRequest::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* AddRequest::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return AddRequest_descriptor_;
}

const AddRequest& AddRequest::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_volume_5fcmdpb_2eproto();
  return *default_instance_;
}

AddRequest* AddRequest::default_instance_ = NULL;

AddRequest* AddRequest::New() const {
  return new AddRequest;
}

void AddRequest::Clear() {
  count_ = GOOGLE_ULONGLONG(0);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool AddRequest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:volume_cmdpb.AddRequest)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional uint64 count = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &count_)));
          set_has_count();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:volume_cmdpb.AddRequest)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:volume_cmdpb.AddRequest)
  return false;
#undef DO_
}

void AddRequest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:volume_cmdpb.AddRequest)
  // optional uint64 count = 1;
  if (has_count()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(1, this->count(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:volume_cmdpb.AddRequest)
}

::google::protobuf::uint8* AddRequest::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:volume_cmdpb.AddRequest)
  // optional uint64 count = 1;
  if (has_count()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt64ToArray(1, this->count(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:volume_cmdpb.AddRequest)
  return target;
}

int AddRequest::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional uint64 count = 1;
    if (has_count()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->count());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void AddRequest::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const AddRequest* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const AddRequest*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void AddRequest::MergeFrom(const AddRequest& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_count()) {
      set_count(from.count());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void AddRequest::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void AddRequest::CopyFrom(const AddRequest& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool AddRequest::IsInitialized() const {

  return true;
}

void AddRequest::Swap(AddRequest* other) {
  if (other != this) {
    std::swap(count_, other->count_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata AddRequest::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = AddRequest_descriptor_;
  metadata.reflection = AddRequest_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int AddResponse::kVolumeIdsFieldNumber;
#endif  // !_MSC_VER

AddResponse::AddResponse()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:volume_cmdpb.AddResponse)
}

void AddResponse::InitAsDefaultInstance() {
}

AddResponse::AddResponse(const AddResponse& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:volume_cmdpb.AddResponse)
}

void AddResponse::SharedCtor() {
  _cached_size_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

AddResponse::~AddResponse() {
  // @@protoc_insertion_point(destructor:volume_cmdpb.AddResponse)
  SharedDtor();
}

void AddResponse::SharedDtor() {
  if (this != default_instance_) {
  }
}

void AddResponse::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* AddResponse::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return AddResponse_descriptor_;
}

const AddResponse& AddResponse::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_volume_5fcmdpb_2eproto();
  return *default_instance_;
}

AddResponse* AddResponse::default_instance_ = NULL;

AddResponse* AddResponse::New() const {
  return new AddResponse;
}

void AddResponse::Clear() {
  volume_ids_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool AddResponse::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:volume_cmdpb.AddResponse)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated uint64 volume_ids = 1;
      case 1: {
        if (tag == 8) {
         parse_volume_ids:
          DO_((::google::protobuf::internal::WireFormatLite::ReadRepeatedPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 1, 8, input, this->mutable_volume_ids())));
        } else if (tag == 10) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPackedPrimitiveNoInline<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, this->mutable_volume_ids())));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(8)) goto parse_volume_ids;
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:volume_cmdpb.AddResponse)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:volume_cmdpb.AddResponse)
  return false;
#undef DO_
}

void AddResponse::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:volume_cmdpb.AddResponse)
  // repeated uint64 volume_ids = 1;
  for (int i = 0; i < this->volume_ids_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(
      1, this->volume_ids(i), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:volume_cmdpb.AddResponse)
}

::google::protobuf::uint8* AddResponse::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:volume_cmdpb.AddResponse)
  // repeated uint64 volume_ids = 1;
  for (int i = 0; i < this->volume_ids_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteUInt64ToArray(1, this->volume_ids(i), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:volume_cmdpb.AddResponse)
  return target;
}

int AddResponse::ByteSize() const {
  int total_size = 0;

  // repeated uint64 volume_ids = 1;
  {
    int data_size = 0;
    for (int i = 0; i < this->volume_ids_size(); i++) {
      data_size += ::google::protobuf::internal::WireFormatLite::
        UInt64Size(this->volume_ids(i));
    }
    total_size += 1 * this->volume_ids_size() + data_size;
  }

  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void AddResponse::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const AddResponse* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const AddResponse*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void AddResponse::MergeFrom(const AddResponse& from) {
  GOOGLE_CHECK_NE(&from, this);
  volume_ids_.MergeFrom(from.volume_ids_);
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void AddResponse::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void AddResponse::CopyFrom(const AddResponse& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool AddResponse::IsInitialized() const {

  return true;
}

void AddResponse::Swap(AddResponse* other) {
  if (other != this) {
    volume_ids_.Swap(&other->volume_ids_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata AddResponse::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = AddResponse_descriptor_;
  metadata.reflection = AddResponse_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int Request::kCmdTypeFieldNumber;
const int Request::kAddFieldNumber;
#endif  // !_MSC_VER

Request::Request()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:volume_cmdpb.Request)
}

void Request::InitAsDefaultInstance() {
  add_ = const_cast< ::volume_cmdpb::AddRequest*>(&::volume_cmdpb::AddRequest::default_instance());
}

Request::Request(const Request& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:volume_cmdpb.Request)
}

void Request::SharedCtor() {
  _cached_size_ = 0;
  cmd_type_ = 0;
  add_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Request::~Request() {
  // @@protoc_insertion_point(destructor:volume_cmdpb.Request)
  SharedDtor();
}

void Request::SharedDtor() {
  if (this != default_instance_) {
    delete add_;
  }
}

void Request::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Request::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Request_descriptor_;
}

const Request& Request::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_volume_5fcmdpb_2eproto();
  return *default_instance_;
}

Request* Request::default_instance_ = NULL;

Request* Request::New() const {
  return new Request;
}

void Request::Clear() {
  if (_has_bits_[0 / 32] & 3) {
    cmd_type_ = 0;
    if (has_add()) {
      if (add_ != NULL) add_->::volume_cmdpb::AddRequest::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Request::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:volume_cmdpb.Request)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional .volume_cmdpb.CmdType cmd_type = 1;
      case 1: {
        if (tag == 8) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::volume_cmdpb::CmdType_IsValid(value)) {
            set_cmd_type(static_cast< ::volume_cmdpb::CmdType >(value));
          } else {
            mutable_unknown_fields()->AddVarint(1, value);
          }
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_add;
        break;
      }

      // optional .volume_cmdpb.AddRequest add = 2;
      case 2: {
        if (tag == 18) {
         parse_add:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_add()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:volume_cmdpb.Request)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:volume_cmdpb.Request)
  return false;
#undef DO_
}

void Request::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:volume_cmdpb.Request)
  // optional .volume_cmdpb.CmdType cmd_type = 1;
  if (has_cmd_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->cmd_type(), output);
  }

  // optional .volume_cmdpb.AddRequest add = 2;
  if (has_add()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->add(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:volume_cmdpb.Request)
}

::google::protobuf::uint8* Request::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:volume_cmdpb.Request)
  // optional .volume_cmdpb.CmdType cmd_type = 1;
  if (has_cmd_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      1, this->cmd_type(), target);
  }

  // optional .volume_cmdpb.AddRequest add = 2;
  if (has_add()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->add(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:volume_cmdpb.Request)
  return target;
}

int Request::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional .volume_cmdpb.CmdType cmd_type = 1;
    if (has_cmd_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->cmd_type());
    }

    // optional .volume_cmdpb.AddRequest add = 2;
    if (has_add()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->add());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Request::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Request* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Request*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Request::MergeFrom(const Request& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_cmd_type()) {
      set_cmd_type(from.cmd_type());
    }
    if (from.has_add()) {
      mutable_add()->::volume_cmdpb::AddRequest::MergeFrom(from.add());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Request::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Request::CopyFrom(const Request& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Request::IsInitialized() const {

  return true;
}

void Request::Swap(Request* other) {
  if (other != this) {
    std::swap(cmd_type_, other->cmd_type_);
    std::swap(add_, other->add_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Request::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Request_descriptor_;
  metadata.reflection = Request_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int Response::kCmdTypeFieldNumber;
const int Response::kAddFieldNumber;
#endif  // !_MSC_VER

Response::Response()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:volume_cmdpb.Response)
}

void Response::InitAsDefaultInstance() {
  add_ = const_cast< ::volume_cmdpb::AddResponse*>(&::volume_cmdpb::AddResponse::default_instance());
}

Response::Response(const Response& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:volume_cmdpb.Response)
}

void Response::SharedCtor() {
  _cached_size_ = 0;
  cmd_type_ = 0;
  add_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Response::~Response() {
  // @@protoc_insertion_point(destructor:volume_cmdpb.Response)
  SharedDtor();
}

void Response::SharedDtor() {
  if (this != default_instance_) {
    delete add_;
  }
}

void Response::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Response::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Response_descriptor_;
}

const Response& Response::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_volume_5fcmdpb_2eproto();
  return *default_instance_;
}

Response* Response::default_instance_ = NULL;

Response* Response::New() const {
  return new Response;
}

void Response::Clear() {
  if (_has_bits_[0 / 32] & 3) {
    cmd_type_ = 0;
    if (has_add()) {
      if (add_ != NULL) add_->::volume_cmdpb::AddResponse::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Response::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:volume_cmdpb.Response)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional .volume_cmdpb.CmdType cmd_type = 1;
      case 1: {
        if (tag == 8) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::volume_cmdpb::CmdType_IsValid(value)) {
            set_cmd_type(static_cast< ::volume_cmdpb::CmdType >(value));
          } else {
            mutable_unknown_fields()->AddVarint(1, value);
          }
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_add;
        break;
      }

      // optional .volume_cmdpb.AddResponse add = 2;
      case 2: {
        if (tag == 18) {
         parse_add:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_add()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:volume_cmdpb.Response)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:volume_cmdpb.Response)
  return false;
#undef DO_
}

void Response::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:volume_cmdpb.Response)
  // optional .volume_cmdpb.CmdType cmd_type = 1;
  if (has_cmd_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->cmd_type(), output);
  }

  // optional .volume_cmdpb.AddResponse add = 2;
  if (has_add()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->add(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:volume_cmdpb.Response)
}

::google::protobuf::uint8* Response::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:volume_cmdpb.Response)
  // optional .volume_cmdpb.CmdType cmd_type = 1;
  if (has_cmd_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      1, this->cmd_type(), target);
  }

  // optional .volume_cmdpb.AddResponse add = 2;
  if (has_add()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->add(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:volume_cmdpb.Response)
  return target;
}

int Response::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional .volume_cmdpb.CmdType cmd_type = 1;
    if (has_cmd_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->cmd_type());
    }

    // optional .volume_cmdpb.AddResponse add = 2;
    if (has_add()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->add());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Response::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Response* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Response*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Response::MergeFrom(const Response& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_cmd_type()) {
      set_cmd_type(from.cmd_type());
    }
    if (from.has_add()) {
      mutable_add()->::volume_cmdpb::AddResponse::MergeFrom(from.add());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Response::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Response::CopyFrom(const Response& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Response::IsInitialized() const {

  return true;
}

void Response::Swap(Response* other) {
  if (other != this) {
    std::swap(cmd_type_, other->cmd_type_);
    std::swap(add_, other->add_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Response::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Response_descriptor_;
  metadata.reflection = Response_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace volume_cmdpb

// @@protoc_insertion_point(global_scope)
