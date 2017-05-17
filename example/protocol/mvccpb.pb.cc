// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mvccpb.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "mvccpb.pb.h"

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

namespace mvccpb {

namespace {

const ::google::protobuf::Descriptor* MetaItem_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  MetaItem_reflection_ = NULL;
const ::google::protobuf::Descriptor* MetaLock_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  MetaLock_reflection_ = NULL;
const ::google::protobuf::Descriptor* Meta_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Meta_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* MetaLockType_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_mvccpb_2eproto() {
  protobuf_AddDesc_mvccpb_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "mvccpb.proto");
  GOOGLE_CHECK(file != NULL);
  MetaItem_descriptor_ = file->message_type(0);
  static const int MetaItem_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MetaItem, start_ts_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MetaItem, commit_ts_),
  };
  MetaItem_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      MetaItem_descriptor_,
      MetaItem::default_instance_,
      MetaItem_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MetaItem, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MetaItem, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(MetaItem));
  MetaLock_descriptor_ = file->message_type(1);
  static const int MetaLock_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MetaLock, type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MetaLock, start_ts_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MetaLock, primary_key_),
  };
  MetaLock_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      MetaLock_descriptor_,
      MetaLock::default_instance_,
      MetaLock_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MetaLock, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MetaLock, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(MetaLock));
  Meta_descriptor_ = file->message_type(2);
  static const int Meta_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Meta, lock_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Meta, items_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Meta, next_),
  };
  Meta_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Meta_descriptor_,
      Meta::default_instance_,
      Meta_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Meta, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Meta, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Meta));
  MetaLockType_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_mvccpb_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    MetaItem_descriptor_, &MetaItem::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    MetaLock_descriptor_, &MetaLock::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Meta_descriptor_, &Meta::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_mvccpb_2eproto() {
  delete MetaItem::default_instance_;
  delete MetaItem_reflection_;
  delete MetaLock::default_instance_;
  delete MetaLock_reflection_;
  delete Meta::default_instance_;
  delete Meta_reflection_;
}

void protobuf_AddDesc_mvccpb_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\014mvccpb.proto\022\006mvccpb\"/\n\010MetaItem\022\020\n\010st"
    "art_ts\030\001 \001(\004\022\021\n\tcommit_ts\030\002 \001(\004\"U\n\010MetaL"
    "ock\022\"\n\004type\030\001 \001(\0162\024.mvccpb.MetaLockType\022"
    "\020\n\010start_ts\030\002 \001(\004\022\023\n\013primary_key\030\003 \001(\014\"U"
    "\n\004Meta\022\036\n\004lock\030\001 \001(\0132\020.mvccpb.MetaLock\022\037"
    "\n\005items\030\002 \003(\0132\020.mvccpb.MetaItem\022\014\n\004next\030"
    "\003 \001(\004*+\n\014MetaLockType\022\014\n\010ReadOnly\020\001\022\r\n\tR"
    "eadWrite\020\002", 290);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "mvccpb.proto", &protobuf_RegisterTypes);
  MetaItem::default_instance_ = new MetaItem();
  MetaLock::default_instance_ = new MetaLock();
  Meta::default_instance_ = new Meta();
  MetaItem::default_instance_->InitAsDefaultInstance();
  MetaLock::default_instance_->InitAsDefaultInstance();
  Meta::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_mvccpb_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_mvccpb_2eproto {
  StaticDescriptorInitializer_mvccpb_2eproto() {
    protobuf_AddDesc_mvccpb_2eproto();
  }
} static_descriptor_initializer_mvccpb_2eproto_;
const ::google::protobuf::EnumDescriptor* MetaLockType_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return MetaLockType_descriptor_;
}
bool MetaLockType_IsValid(int value) {
  switch(value) {
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}


// ===================================================================

#ifndef _MSC_VER
const int MetaItem::kStartTsFieldNumber;
const int MetaItem::kCommitTsFieldNumber;
#endif  // !_MSC_VER

MetaItem::MetaItem()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:mvccpb.MetaItem)
}

void MetaItem::InitAsDefaultInstance() {
}

MetaItem::MetaItem(const MetaItem& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:mvccpb.MetaItem)
}

void MetaItem::SharedCtor() {
  _cached_size_ = 0;
  start_ts_ = GOOGLE_ULONGLONG(0);
  commit_ts_ = GOOGLE_ULONGLONG(0);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

MetaItem::~MetaItem() {
  // @@protoc_insertion_point(destructor:mvccpb.MetaItem)
  SharedDtor();
}

void MetaItem::SharedDtor() {
  if (this != default_instance_) {
  }
}

void MetaItem::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* MetaItem::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return MetaItem_descriptor_;
}

const MetaItem& MetaItem::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_mvccpb_2eproto();
  return *default_instance_;
}

MetaItem* MetaItem::default_instance_ = NULL;

MetaItem* MetaItem::New() const {
  return new MetaItem;
}

void MetaItem::Clear() {
#define OFFSET_OF_FIELD_(f) (reinterpret_cast<char*>(      \
  &reinterpret_cast<MetaItem*>(16)->f) - \
   reinterpret_cast<char*>(16))

#define ZR_(first, last) do {                              \
    size_t f = OFFSET_OF_FIELD_(first);                    \
    size_t n = OFFSET_OF_FIELD_(last) - f + sizeof(last);  \
    ::memset(&first, 0, n);                                \
  } while (0)

  ZR_(start_ts_, commit_ts_);

#undef OFFSET_OF_FIELD_
#undef ZR_

  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool MetaItem::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:mvccpb.MetaItem)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional uint64 start_ts = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &start_ts_)));
          set_has_start_ts();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(16)) goto parse_commit_ts;
        break;
      }

      // optional uint64 commit_ts = 2;
      case 2: {
        if (tag == 16) {
         parse_commit_ts:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &commit_ts_)));
          set_has_commit_ts();
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
  // @@protoc_insertion_point(parse_success:mvccpb.MetaItem)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:mvccpb.MetaItem)
  return false;
#undef DO_
}

void MetaItem::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:mvccpb.MetaItem)
  // optional uint64 start_ts = 1;
  if (has_start_ts()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(1, this->start_ts(), output);
  }

  // optional uint64 commit_ts = 2;
  if (has_commit_ts()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(2, this->commit_ts(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:mvccpb.MetaItem)
}

::google::protobuf::uint8* MetaItem::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:mvccpb.MetaItem)
  // optional uint64 start_ts = 1;
  if (has_start_ts()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt64ToArray(1, this->start_ts(), target);
  }

  // optional uint64 commit_ts = 2;
  if (has_commit_ts()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt64ToArray(2, this->commit_ts(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:mvccpb.MetaItem)
  return target;
}

int MetaItem::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional uint64 start_ts = 1;
    if (has_start_ts()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->start_ts());
    }

    // optional uint64 commit_ts = 2;
    if (has_commit_ts()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->commit_ts());
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

void MetaItem::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const MetaItem* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const MetaItem*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void MetaItem::MergeFrom(const MetaItem& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_start_ts()) {
      set_start_ts(from.start_ts());
    }
    if (from.has_commit_ts()) {
      set_commit_ts(from.commit_ts());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void MetaItem::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void MetaItem::CopyFrom(const MetaItem& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool MetaItem::IsInitialized() const {

  return true;
}

void MetaItem::Swap(MetaItem* other) {
  if (other != this) {
    std::swap(start_ts_, other->start_ts_);
    std::swap(commit_ts_, other->commit_ts_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata MetaItem::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = MetaItem_descriptor_;
  metadata.reflection = MetaItem_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int MetaLock::kTypeFieldNumber;
const int MetaLock::kStartTsFieldNumber;
const int MetaLock::kPrimaryKeyFieldNumber;
#endif  // !_MSC_VER

MetaLock::MetaLock()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:mvccpb.MetaLock)
}

void MetaLock::InitAsDefaultInstance() {
}

MetaLock::MetaLock(const MetaLock& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:mvccpb.MetaLock)
}

void MetaLock::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  type_ = 1;
  start_ts_ = GOOGLE_ULONGLONG(0);
  primary_key_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

MetaLock::~MetaLock() {
  // @@protoc_insertion_point(destructor:mvccpb.MetaLock)
  SharedDtor();
}

void MetaLock::SharedDtor() {
  if (primary_key_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete primary_key_;
  }
  if (this != default_instance_) {
  }
}

void MetaLock::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* MetaLock::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return MetaLock_descriptor_;
}

const MetaLock& MetaLock::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_mvccpb_2eproto();
  return *default_instance_;
}

MetaLock* MetaLock::default_instance_ = NULL;

MetaLock* MetaLock::New() const {
  return new MetaLock;
}

void MetaLock::Clear() {
  if (_has_bits_[0 / 32] & 7) {
    type_ = 1;
    start_ts_ = GOOGLE_ULONGLONG(0);
    if (has_primary_key()) {
      if (primary_key_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        primary_key_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool MetaLock::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:mvccpb.MetaLock)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional .mvccpb.MetaLockType type = 1;
      case 1: {
        if (tag == 8) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::mvccpb::MetaLockType_IsValid(value)) {
            set_type(static_cast< ::mvccpb::MetaLockType >(value));
          } else {
            mutable_unknown_fields()->AddVarint(1, value);
          }
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(16)) goto parse_start_ts;
        break;
      }

      // optional uint64 start_ts = 2;
      case 2: {
        if (tag == 16) {
         parse_start_ts:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &start_ts_)));
          set_has_start_ts();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(26)) goto parse_primary_key;
        break;
      }

      // optional bytes primary_key = 3;
      case 3: {
        if (tag == 26) {
         parse_primary_key:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_primary_key()));
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
  // @@protoc_insertion_point(parse_success:mvccpb.MetaLock)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:mvccpb.MetaLock)
  return false;
#undef DO_
}

void MetaLock::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:mvccpb.MetaLock)
  // optional .mvccpb.MetaLockType type = 1;
  if (has_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->type(), output);
  }

  // optional uint64 start_ts = 2;
  if (has_start_ts()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(2, this->start_ts(), output);
  }

  // optional bytes primary_key = 3;
  if (has_primary_key()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytesMaybeAliased(
      3, this->primary_key(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:mvccpb.MetaLock)
}

::google::protobuf::uint8* MetaLock::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:mvccpb.MetaLock)
  // optional .mvccpb.MetaLockType type = 1;
  if (has_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      1, this->type(), target);
  }

  // optional uint64 start_ts = 2;
  if (has_start_ts()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt64ToArray(2, this->start_ts(), target);
  }

  // optional bytes primary_key = 3;
  if (has_primary_key()) {
    target =
      ::google::protobuf::internal::WireFormatLite::WriteBytesToArray(
        3, this->primary_key(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:mvccpb.MetaLock)
  return target;
}

int MetaLock::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional .mvccpb.MetaLockType type = 1;
    if (has_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->type());
    }

    // optional uint64 start_ts = 2;
    if (has_start_ts()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->start_ts());
    }

    // optional bytes primary_key = 3;
    if (has_primary_key()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->primary_key());
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

void MetaLock::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const MetaLock* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const MetaLock*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void MetaLock::MergeFrom(const MetaLock& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_type()) {
      set_type(from.type());
    }
    if (from.has_start_ts()) {
      set_start_ts(from.start_ts());
    }
    if (from.has_primary_key()) {
      set_primary_key(from.primary_key());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void MetaLock::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void MetaLock::CopyFrom(const MetaLock& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool MetaLock::IsInitialized() const {

  return true;
}

void MetaLock::Swap(MetaLock* other) {
  if (other != this) {
    std::swap(type_, other->type_);
    std::swap(start_ts_, other->start_ts_);
    std::swap(primary_key_, other->primary_key_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata MetaLock::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = MetaLock_descriptor_;
  metadata.reflection = MetaLock_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int Meta::kLockFieldNumber;
const int Meta::kItemsFieldNumber;
const int Meta::kNextFieldNumber;
#endif  // !_MSC_VER

Meta::Meta()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:mvccpb.Meta)
}

void Meta::InitAsDefaultInstance() {
  lock_ = const_cast< ::mvccpb::MetaLock*>(&::mvccpb::MetaLock::default_instance());
}

Meta::Meta(const Meta& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:mvccpb.Meta)
}

void Meta::SharedCtor() {
  _cached_size_ = 0;
  lock_ = NULL;
  next_ = GOOGLE_ULONGLONG(0);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Meta::~Meta() {
  // @@protoc_insertion_point(destructor:mvccpb.Meta)
  SharedDtor();
}

void Meta::SharedDtor() {
  if (this != default_instance_) {
    delete lock_;
  }
}

void Meta::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Meta::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Meta_descriptor_;
}

const Meta& Meta::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_mvccpb_2eproto();
  return *default_instance_;
}

Meta* Meta::default_instance_ = NULL;

Meta* Meta::New() const {
  return new Meta;
}

void Meta::Clear() {
  if (_has_bits_[0 / 32] & 5) {
    if (has_lock()) {
      if (lock_ != NULL) lock_->::mvccpb::MetaLock::Clear();
    }
    next_ = GOOGLE_ULONGLONG(0);
  }
  items_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Meta::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:mvccpb.Meta)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional .mvccpb.MetaLock lock = 1;
      case 1: {
        if (tag == 10) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_lock()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_items;
        break;
      }

      // repeated .mvccpb.MetaItem items = 2;
      case 2: {
        if (tag == 18) {
         parse_items:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_items()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_items;
        if (input->ExpectTag(24)) goto parse_next;
        break;
      }

      // optional uint64 next = 3;
      case 3: {
        if (tag == 24) {
         parse_next:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &next_)));
          set_has_next();
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
  // @@protoc_insertion_point(parse_success:mvccpb.Meta)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:mvccpb.Meta)
  return false;
#undef DO_
}

void Meta::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:mvccpb.Meta)
  // optional .mvccpb.MetaLock lock = 1;
  if (has_lock()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->lock(), output);
  }

  // repeated .mvccpb.MetaItem items = 2;
  for (int i = 0; i < this->items_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->items(i), output);
  }

  // optional uint64 next = 3;
  if (has_next()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(3, this->next(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:mvccpb.Meta)
}

::google::protobuf::uint8* Meta::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:mvccpb.Meta)
  // optional .mvccpb.MetaLock lock = 1;
  if (has_lock()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->lock(), target);
  }

  // repeated .mvccpb.MetaItem items = 2;
  for (int i = 0; i < this->items_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->items(i), target);
  }

  // optional uint64 next = 3;
  if (has_next()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt64ToArray(3, this->next(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:mvccpb.Meta)
  return target;
}

int Meta::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional .mvccpb.MetaLock lock = 1;
    if (has_lock()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->lock());
    }

    // optional uint64 next = 3;
    if (has_next()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->next());
    }

  }
  // repeated .mvccpb.MetaItem items = 2;
  total_size += 1 * this->items_size();
  for (int i = 0; i < this->items_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->items(i));
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

void Meta::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Meta* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Meta*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Meta::MergeFrom(const Meta& from) {
  GOOGLE_CHECK_NE(&from, this);
  items_.MergeFrom(from.items_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_lock()) {
      mutable_lock()->::mvccpb::MetaLock::MergeFrom(from.lock());
    }
    if (from.has_next()) {
      set_next(from.next());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Meta::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Meta::CopyFrom(const Meta& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Meta::IsInitialized() const {

  return true;
}

void Meta::Swap(Meta* other) {
  if (other != this) {
    std::swap(lock_, other->lock_);
    items_.Swap(&other->items_);
    std::swap(next_, other->next_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Meta::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Meta_descriptor_;
  metadata.reflection = Meta_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace mvccpb

// @@protoc_insertion_point(global_scope)
