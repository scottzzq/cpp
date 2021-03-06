// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mvccpb.proto

#ifndef PROTOBUF_mvccpb_2eproto__INCLUDED
#define PROTOBUF_mvccpb_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace mvccpb {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_mvccpb_2eproto();
void protobuf_AssignDesc_mvccpb_2eproto();
void protobuf_ShutdownFile_mvccpb_2eproto();

class MetaItem;
class MetaLock;
class Meta;

enum MetaLockType {
  ReadOnly = 1,
  ReadWrite = 2
};
bool MetaLockType_IsValid(int value);
const MetaLockType MetaLockType_MIN = ReadOnly;
const MetaLockType MetaLockType_MAX = ReadWrite;
const int MetaLockType_ARRAYSIZE = MetaLockType_MAX + 1;

const ::google::protobuf::EnumDescriptor* MetaLockType_descriptor();
inline const ::std::string& MetaLockType_Name(MetaLockType value) {
  return ::google::protobuf::internal::NameOfEnum(
    MetaLockType_descriptor(), value);
}
inline bool MetaLockType_Parse(
    const ::std::string& name, MetaLockType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<MetaLockType>(
    MetaLockType_descriptor(), name, value);
}
// ===================================================================

class MetaItem : public ::google::protobuf::Message {
 public:
  MetaItem();
  virtual ~MetaItem();

  MetaItem(const MetaItem& from);

  inline MetaItem& operator=(const MetaItem& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const MetaItem& default_instance();

  void Swap(MetaItem* other);

  // implements Message ----------------------------------------------

  MetaItem* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const MetaItem& from);
  void MergeFrom(const MetaItem& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional uint64 start_ts = 1;
  inline bool has_start_ts() const;
  inline void clear_start_ts();
  static const int kStartTsFieldNumber = 1;
  inline ::google::protobuf::uint64 start_ts() const;
  inline void set_start_ts(::google::protobuf::uint64 value);

  // optional uint64 commit_ts = 2;
  inline bool has_commit_ts() const;
  inline void clear_commit_ts();
  static const int kCommitTsFieldNumber = 2;
  inline ::google::protobuf::uint64 commit_ts() const;
  inline void set_commit_ts(::google::protobuf::uint64 value);

  // @@protoc_insertion_point(class_scope:mvccpb.MetaItem)
 private:
  inline void set_has_start_ts();
  inline void clear_has_start_ts();
  inline void set_has_commit_ts();
  inline void clear_has_commit_ts();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::uint64 start_ts_;
  ::google::protobuf::uint64 commit_ts_;
  friend void  protobuf_AddDesc_mvccpb_2eproto();
  friend void protobuf_AssignDesc_mvccpb_2eproto();
  friend void protobuf_ShutdownFile_mvccpb_2eproto();

  void InitAsDefaultInstance();
  static MetaItem* default_instance_;
};
// -------------------------------------------------------------------

class MetaLock : public ::google::protobuf::Message {
 public:
  MetaLock();
  virtual ~MetaLock();

  MetaLock(const MetaLock& from);

  inline MetaLock& operator=(const MetaLock& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const MetaLock& default_instance();

  void Swap(MetaLock* other);

  // implements Message ----------------------------------------------

  MetaLock* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const MetaLock& from);
  void MergeFrom(const MetaLock& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional .mvccpb.MetaLockType type = 1;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 1;
  inline ::mvccpb::MetaLockType type() const;
  inline void set_type(::mvccpb::MetaLockType value);

  // optional uint64 start_ts = 2;
  inline bool has_start_ts() const;
  inline void clear_start_ts();
  static const int kStartTsFieldNumber = 2;
  inline ::google::protobuf::uint64 start_ts() const;
  inline void set_start_ts(::google::protobuf::uint64 value);

  // optional bytes primary_key = 3;
  inline bool has_primary_key() const;
  inline void clear_primary_key();
  static const int kPrimaryKeyFieldNumber = 3;
  inline const ::std::string& primary_key() const;
  inline void set_primary_key(const ::std::string& value);
  inline void set_primary_key(const char* value);
  inline void set_primary_key(const void* value, size_t size);
  inline ::std::string* mutable_primary_key();
  inline ::std::string* release_primary_key();
  inline void set_allocated_primary_key(::std::string* primary_key);

  // @@protoc_insertion_point(class_scope:mvccpb.MetaLock)
 private:
  inline void set_has_type();
  inline void clear_has_type();
  inline void set_has_start_ts();
  inline void clear_has_start_ts();
  inline void set_has_primary_key();
  inline void clear_has_primary_key();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::uint64 start_ts_;
  ::std::string* primary_key_;
  int type_;
  friend void  protobuf_AddDesc_mvccpb_2eproto();
  friend void protobuf_AssignDesc_mvccpb_2eproto();
  friend void protobuf_ShutdownFile_mvccpb_2eproto();

  void InitAsDefaultInstance();
  static MetaLock* default_instance_;
};
// -------------------------------------------------------------------

class Meta : public ::google::protobuf::Message {
 public:
  Meta();
  virtual ~Meta();

  Meta(const Meta& from);

  inline Meta& operator=(const Meta& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Meta& default_instance();

  void Swap(Meta* other);

  // implements Message ----------------------------------------------

  Meta* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Meta& from);
  void MergeFrom(const Meta& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional .mvccpb.MetaLock lock = 1;
  inline bool has_lock() const;
  inline void clear_lock();
  static const int kLockFieldNumber = 1;
  inline const ::mvccpb::MetaLock& lock() const;
  inline ::mvccpb::MetaLock* mutable_lock();
  inline ::mvccpb::MetaLock* release_lock();
  inline void set_allocated_lock(::mvccpb::MetaLock* lock);

  // repeated .mvccpb.MetaItem items = 2;
  inline int items_size() const;
  inline void clear_items();
  static const int kItemsFieldNumber = 2;
  inline const ::mvccpb::MetaItem& items(int index) const;
  inline ::mvccpb::MetaItem* mutable_items(int index);
  inline ::mvccpb::MetaItem* add_items();
  inline const ::google::protobuf::RepeatedPtrField< ::mvccpb::MetaItem >&
      items() const;
  inline ::google::protobuf::RepeatedPtrField< ::mvccpb::MetaItem >*
      mutable_items();

  // optional uint64 next = 3;
  inline bool has_next() const;
  inline void clear_next();
  static const int kNextFieldNumber = 3;
  inline ::google::protobuf::uint64 next() const;
  inline void set_next(::google::protobuf::uint64 value);

  // @@protoc_insertion_point(class_scope:mvccpb.Meta)
 private:
  inline void set_has_lock();
  inline void clear_has_lock();
  inline void set_has_next();
  inline void clear_has_next();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::mvccpb::MetaLock* lock_;
  ::google::protobuf::RepeatedPtrField< ::mvccpb::MetaItem > items_;
  ::google::protobuf::uint64 next_;
  friend void  protobuf_AddDesc_mvccpb_2eproto();
  friend void protobuf_AssignDesc_mvccpb_2eproto();
  friend void protobuf_ShutdownFile_mvccpb_2eproto();

  void InitAsDefaultInstance();
  static Meta* default_instance_;
};
// ===================================================================


// ===================================================================

// MetaItem

// optional uint64 start_ts = 1;
inline bool MetaItem::has_start_ts() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void MetaItem::set_has_start_ts() {
  _has_bits_[0] |= 0x00000001u;
}
inline void MetaItem::clear_has_start_ts() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void MetaItem::clear_start_ts() {
  start_ts_ = GOOGLE_ULONGLONG(0);
  clear_has_start_ts();
}
inline ::google::protobuf::uint64 MetaItem::start_ts() const {
  // @@protoc_insertion_point(field_get:mvccpb.MetaItem.start_ts)
  return start_ts_;
}
inline void MetaItem::set_start_ts(::google::protobuf::uint64 value) {
  set_has_start_ts();
  start_ts_ = value;
  // @@protoc_insertion_point(field_set:mvccpb.MetaItem.start_ts)
}

// optional uint64 commit_ts = 2;
inline bool MetaItem::has_commit_ts() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void MetaItem::set_has_commit_ts() {
  _has_bits_[0] |= 0x00000002u;
}
inline void MetaItem::clear_has_commit_ts() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void MetaItem::clear_commit_ts() {
  commit_ts_ = GOOGLE_ULONGLONG(0);
  clear_has_commit_ts();
}
inline ::google::protobuf::uint64 MetaItem::commit_ts() const {
  // @@protoc_insertion_point(field_get:mvccpb.MetaItem.commit_ts)
  return commit_ts_;
}
inline void MetaItem::set_commit_ts(::google::protobuf::uint64 value) {
  set_has_commit_ts();
  commit_ts_ = value;
  // @@protoc_insertion_point(field_set:mvccpb.MetaItem.commit_ts)
}

// -------------------------------------------------------------------

// MetaLock

// optional .mvccpb.MetaLockType type = 1;
inline bool MetaLock::has_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void MetaLock::set_has_type() {
  _has_bits_[0] |= 0x00000001u;
}
inline void MetaLock::clear_has_type() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void MetaLock::clear_type() {
  type_ = 1;
  clear_has_type();
}
inline ::mvccpb::MetaLockType MetaLock::type() const {
  // @@protoc_insertion_point(field_get:mvccpb.MetaLock.type)
  return static_cast< ::mvccpb::MetaLockType >(type_);
}
inline void MetaLock::set_type(::mvccpb::MetaLockType value) {
  assert(::mvccpb::MetaLockType_IsValid(value));
  set_has_type();
  type_ = value;
  // @@protoc_insertion_point(field_set:mvccpb.MetaLock.type)
}

// optional uint64 start_ts = 2;
inline bool MetaLock::has_start_ts() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void MetaLock::set_has_start_ts() {
  _has_bits_[0] |= 0x00000002u;
}
inline void MetaLock::clear_has_start_ts() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void MetaLock::clear_start_ts() {
  start_ts_ = GOOGLE_ULONGLONG(0);
  clear_has_start_ts();
}
inline ::google::protobuf::uint64 MetaLock::start_ts() const {
  // @@protoc_insertion_point(field_get:mvccpb.MetaLock.start_ts)
  return start_ts_;
}
inline void MetaLock::set_start_ts(::google::protobuf::uint64 value) {
  set_has_start_ts();
  start_ts_ = value;
  // @@protoc_insertion_point(field_set:mvccpb.MetaLock.start_ts)
}

// optional bytes primary_key = 3;
inline bool MetaLock::has_primary_key() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void MetaLock::set_has_primary_key() {
  _has_bits_[0] |= 0x00000004u;
}
inline void MetaLock::clear_has_primary_key() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void MetaLock::clear_primary_key() {
  if (primary_key_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    primary_key_->clear();
  }
  clear_has_primary_key();
}
inline const ::std::string& MetaLock::primary_key() const {
  // @@protoc_insertion_point(field_get:mvccpb.MetaLock.primary_key)
  return *primary_key_;
}
inline void MetaLock::set_primary_key(const ::std::string& value) {
  set_has_primary_key();
  if (primary_key_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    primary_key_ = new ::std::string;
  }
  primary_key_->assign(value);
  // @@protoc_insertion_point(field_set:mvccpb.MetaLock.primary_key)
}
inline void MetaLock::set_primary_key(const char* value) {
  set_has_primary_key();
  if (primary_key_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    primary_key_ = new ::std::string;
  }
  primary_key_->assign(value);
  // @@protoc_insertion_point(field_set_char:mvccpb.MetaLock.primary_key)
}
inline void MetaLock::set_primary_key(const void* value, size_t size) {
  set_has_primary_key();
  if (primary_key_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    primary_key_ = new ::std::string;
  }
  primary_key_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:mvccpb.MetaLock.primary_key)
}
inline ::std::string* MetaLock::mutable_primary_key() {
  set_has_primary_key();
  if (primary_key_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    primary_key_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:mvccpb.MetaLock.primary_key)
  return primary_key_;
}
inline ::std::string* MetaLock::release_primary_key() {
  clear_has_primary_key();
  if (primary_key_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = primary_key_;
    primary_key_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void MetaLock::set_allocated_primary_key(::std::string* primary_key) {
  if (primary_key_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete primary_key_;
  }
  if (primary_key) {
    set_has_primary_key();
    primary_key_ = primary_key;
  } else {
    clear_has_primary_key();
    primary_key_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:mvccpb.MetaLock.primary_key)
}

// -------------------------------------------------------------------

// Meta

// optional .mvccpb.MetaLock lock = 1;
inline bool Meta::has_lock() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Meta::set_has_lock() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Meta::clear_has_lock() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Meta::clear_lock() {
  if (lock_ != NULL) lock_->::mvccpb::MetaLock::Clear();
  clear_has_lock();
}
inline const ::mvccpb::MetaLock& Meta::lock() const {
  // @@protoc_insertion_point(field_get:mvccpb.Meta.lock)
  return lock_ != NULL ? *lock_ : *default_instance_->lock_;
}
inline ::mvccpb::MetaLock* Meta::mutable_lock() {
  set_has_lock();
  if (lock_ == NULL) lock_ = new ::mvccpb::MetaLock;
  // @@protoc_insertion_point(field_mutable:mvccpb.Meta.lock)
  return lock_;
}
inline ::mvccpb::MetaLock* Meta::release_lock() {
  clear_has_lock();
  ::mvccpb::MetaLock* temp = lock_;
  lock_ = NULL;
  return temp;
}
inline void Meta::set_allocated_lock(::mvccpb::MetaLock* lock) {
  delete lock_;
  lock_ = lock;
  if (lock) {
    set_has_lock();
  } else {
    clear_has_lock();
  }
  // @@protoc_insertion_point(field_set_allocated:mvccpb.Meta.lock)
}

// repeated .mvccpb.MetaItem items = 2;
inline int Meta::items_size() const {
  return items_.size();
}
inline void Meta::clear_items() {
  items_.Clear();
}
inline const ::mvccpb::MetaItem& Meta::items(int index) const {
  // @@protoc_insertion_point(field_get:mvccpb.Meta.items)
  return items_.Get(index);
}
inline ::mvccpb::MetaItem* Meta::mutable_items(int index) {
  // @@protoc_insertion_point(field_mutable:mvccpb.Meta.items)
  return items_.Mutable(index);
}
inline ::mvccpb::MetaItem* Meta::add_items() {
  // @@protoc_insertion_point(field_add:mvccpb.Meta.items)
  return items_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::mvccpb::MetaItem >&
Meta::items() const {
  // @@protoc_insertion_point(field_list:mvccpb.Meta.items)
  return items_;
}
inline ::google::protobuf::RepeatedPtrField< ::mvccpb::MetaItem >*
Meta::mutable_items() {
  // @@protoc_insertion_point(field_mutable_list:mvccpb.Meta.items)
  return &items_;
}

// optional uint64 next = 3;
inline bool Meta::has_next() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Meta::set_has_next() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Meta::clear_has_next() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Meta::clear_next() {
  next_ = GOOGLE_ULONGLONG(0);
  clear_has_next();
}
inline ::google::protobuf::uint64 Meta::next() const {
  // @@protoc_insertion_point(field_get:mvccpb.Meta.next)
  return next_;
}
inline void Meta::set_next(::google::protobuf::uint64 value) {
  set_has_next();
  next_ = value;
  // @@protoc_insertion_point(field_set:mvccpb.Meta.next)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace mvccpb

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::mvccpb::MetaLockType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::mvccpb::MetaLockType>() {
  return ::mvccpb::MetaLockType_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_mvccpb_2eproto__INCLUDED
