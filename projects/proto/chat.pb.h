// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: chat.proto

#ifndef PROTOBUF_chat_2eproto__INCLUDED
#define PROTOBUF_chat_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
#include "time.pb.h"
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_chat_2eproto();
void protobuf_AssignDesc_chat_2eproto();
void protobuf_ShutdownFile_chat_2eproto();

class Chat;

// ===================================================================

class Chat : public ::google::protobuf::Message {
 public:
  Chat();
  virtual ~Chat();

  Chat(const Chat& from);

  inline Chat& operator=(const Chat& from) {
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
  static const Chat& default_instance();

  void Swap(Chat* other);

  // implements Message ----------------------------------------------

  Chat* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Chat& from);
  void MergeFrom(const Chat& from);
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

  // optional int32 chatType = 1;
  inline bool has_chattype() const;
  inline void clear_chattype();
  static const int kChatTypeFieldNumber = 1;
  inline ::google::protobuf::int32 chattype() const;
  inline void set_chattype(::google::protobuf::int32 value);

  // optional string fromPlayerName = 2;
  inline bool has_fromplayername() const;
  inline void clear_fromplayername();
  static const int kFromPlayerNameFieldNumber = 2;
  inline const ::std::string& fromplayername() const;
  inline void set_fromplayername(const ::std::string& value);
  inline void set_fromplayername(const char* value);
  inline void set_fromplayername(const char* value, size_t size);
  inline ::std::string* mutable_fromplayername();
  inline ::std::string* release_fromplayername();
  inline void set_allocated_fromplayername(::std::string* fromplayername);

  // optional int64 fromPlayerOnlyId = 3;
  inline bool has_fromplayeronlyid() const;
  inline void clear_fromplayeronlyid();
  static const int kFromPlayerOnlyIdFieldNumber = 3;
  inline ::google::protobuf::int64 fromplayeronlyid() const;
  inline void set_fromplayeronlyid(::google::protobuf::int64 value);

  // optional string toPlayerName = 4;
  inline bool has_toplayername() const;
  inline void clear_toplayername();
  static const int kToPlayerNameFieldNumber = 4;
  inline const ::std::string& toplayername() const;
  inline void set_toplayername(const ::std::string& value);
  inline void set_toplayername(const char* value);
  inline void set_toplayername(const char* value, size_t size);
  inline ::std::string* mutable_toplayername();
  inline ::std::string* release_toplayername();
  inline void set_allocated_toplayername(::std::string* toplayername);

  // optional int64 toPlayerOnlyId = 5;
  inline bool has_toplayeronlyid() const;
  inline void clear_toplayeronlyid();
  static const int kToPlayerOnlyIdFieldNumber = 5;
  inline ::google::protobuf::int64 toplayeronlyid() const;
  inline void set_toplayeronlyid(::google::protobuf::int64 value);

  // optional string chatMsg = 6;
  inline bool has_chatmsg() const;
  inline void clear_chatmsg();
  static const int kChatMsgFieldNumber = 6;
  inline const ::std::string& chatmsg() const;
  inline void set_chatmsg(const ::std::string& value);
  inline void set_chatmsg(const char* value);
  inline void set_chatmsg(const char* value, size_t size);
  inline ::std::string* mutable_chatmsg();
  inline ::std::string* release_chatmsg();
  inline void set_allocated_chatmsg(::std::string* chatmsg);

  // optional int32 vipLevel = 7;
  inline bool has_viplevel() const;
  inline void clear_viplevel();
  static const int kVipLevelFieldNumber = 7;
  inline ::google::protobuf::int32 viplevel() const;
  inline void set_viplevel(::google::protobuf::int32 value);

  // repeated .TimeInfo info = 8;
  inline int info_size() const;
  inline void clear_info();
  static const int kInfoFieldNumber = 8;
  inline const ::TimeInfo& info(int index) const;
  inline ::TimeInfo* mutable_info(int index);
  inline ::TimeInfo* add_info();
  inline const ::google::protobuf::RepeatedPtrField< ::TimeInfo >&
      info() const;
  inline ::google::protobuf::RepeatedPtrField< ::TimeInfo >*
      mutable_info();

  // @@protoc_insertion_point(class_scope:Chat)
 private:
  inline void set_has_chattype();
  inline void clear_has_chattype();
  inline void set_has_fromplayername();
  inline void clear_has_fromplayername();
  inline void set_has_fromplayeronlyid();
  inline void clear_has_fromplayeronlyid();
  inline void set_has_toplayername();
  inline void clear_has_toplayername();
  inline void set_has_toplayeronlyid();
  inline void clear_has_toplayeronlyid();
  inline void set_has_chatmsg();
  inline void clear_has_chatmsg();
  inline void set_has_viplevel();
  inline void clear_has_viplevel();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* fromplayername_;
  ::google::protobuf::int64 fromplayeronlyid_;
  ::std::string* toplayername_;
  ::google::protobuf::int32 chattype_;
  ::google::protobuf::int32 viplevel_;
  ::google::protobuf::int64 toplayeronlyid_;
  ::std::string* chatmsg_;
  ::google::protobuf::RepeatedPtrField< ::TimeInfo > info_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(8 + 31) / 32];

  friend void  protobuf_AddDesc_chat_2eproto();
  friend void protobuf_AssignDesc_chat_2eproto();
  friend void protobuf_ShutdownFile_chat_2eproto();

  void InitAsDefaultInstance();
  static Chat* default_instance_;
};
// ===================================================================


// ===================================================================

// Chat

// optional int32 chatType = 1;
inline bool Chat::has_chattype() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Chat::set_has_chattype() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Chat::clear_has_chattype() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Chat::clear_chattype() {
  chattype_ = 0;
  clear_has_chattype();
}
inline ::google::protobuf::int32 Chat::chattype() const {
  return chattype_;
}
inline void Chat::set_chattype(::google::protobuf::int32 value) {
  set_has_chattype();
  chattype_ = value;
}

// optional string fromPlayerName = 2;
inline bool Chat::has_fromplayername() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Chat::set_has_fromplayername() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Chat::clear_has_fromplayername() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Chat::clear_fromplayername() {
  if (fromplayername_ != &::google::protobuf::internal::kEmptyString) {
    fromplayername_->clear();
  }
  clear_has_fromplayername();
}
inline const ::std::string& Chat::fromplayername() const {
  return *fromplayername_;
}
inline void Chat::set_fromplayername(const ::std::string& value) {
  set_has_fromplayername();
  if (fromplayername_ == &::google::protobuf::internal::kEmptyString) {
    fromplayername_ = new ::std::string;
  }
  fromplayername_->assign(value);
}
inline void Chat::set_fromplayername(const char* value) {
  set_has_fromplayername();
  if (fromplayername_ == &::google::protobuf::internal::kEmptyString) {
    fromplayername_ = new ::std::string;
  }
  fromplayername_->assign(value);
}
inline void Chat::set_fromplayername(const char* value, size_t size) {
  set_has_fromplayername();
  if (fromplayername_ == &::google::protobuf::internal::kEmptyString) {
    fromplayername_ = new ::std::string;
  }
  fromplayername_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Chat::mutable_fromplayername() {
  set_has_fromplayername();
  if (fromplayername_ == &::google::protobuf::internal::kEmptyString) {
    fromplayername_ = new ::std::string;
  }
  return fromplayername_;
}
inline ::std::string* Chat::release_fromplayername() {
  clear_has_fromplayername();
  if (fromplayername_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = fromplayername_;
    fromplayername_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void Chat::set_allocated_fromplayername(::std::string* fromplayername) {
  if (fromplayername_ != &::google::protobuf::internal::kEmptyString) {
    delete fromplayername_;
  }
  if (fromplayername) {
    set_has_fromplayername();
    fromplayername_ = fromplayername;
  } else {
    clear_has_fromplayername();
    fromplayername_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional int64 fromPlayerOnlyId = 3;
inline bool Chat::has_fromplayeronlyid() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Chat::set_has_fromplayeronlyid() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Chat::clear_has_fromplayeronlyid() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Chat::clear_fromplayeronlyid() {
  fromplayeronlyid_ = GOOGLE_LONGLONG(0);
  clear_has_fromplayeronlyid();
}
inline ::google::protobuf::int64 Chat::fromplayeronlyid() const {
  return fromplayeronlyid_;
}
inline void Chat::set_fromplayeronlyid(::google::protobuf::int64 value) {
  set_has_fromplayeronlyid();
  fromplayeronlyid_ = value;
}

// optional string toPlayerName = 4;
inline bool Chat::has_toplayername() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void Chat::set_has_toplayername() {
  _has_bits_[0] |= 0x00000008u;
}
inline void Chat::clear_has_toplayername() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void Chat::clear_toplayername() {
  if (toplayername_ != &::google::protobuf::internal::kEmptyString) {
    toplayername_->clear();
  }
  clear_has_toplayername();
}
inline const ::std::string& Chat::toplayername() const {
  return *toplayername_;
}
inline void Chat::set_toplayername(const ::std::string& value) {
  set_has_toplayername();
  if (toplayername_ == &::google::protobuf::internal::kEmptyString) {
    toplayername_ = new ::std::string;
  }
  toplayername_->assign(value);
}
inline void Chat::set_toplayername(const char* value) {
  set_has_toplayername();
  if (toplayername_ == &::google::protobuf::internal::kEmptyString) {
    toplayername_ = new ::std::string;
  }
  toplayername_->assign(value);
}
inline void Chat::set_toplayername(const char* value, size_t size) {
  set_has_toplayername();
  if (toplayername_ == &::google::protobuf::internal::kEmptyString) {
    toplayername_ = new ::std::string;
  }
  toplayername_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Chat::mutable_toplayername() {
  set_has_toplayername();
  if (toplayername_ == &::google::protobuf::internal::kEmptyString) {
    toplayername_ = new ::std::string;
  }
  return toplayername_;
}
inline ::std::string* Chat::release_toplayername() {
  clear_has_toplayername();
  if (toplayername_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = toplayername_;
    toplayername_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void Chat::set_allocated_toplayername(::std::string* toplayername) {
  if (toplayername_ != &::google::protobuf::internal::kEmptyString) {
    delete toplayername_;
  }
  if (toplayername) {
    set_has_toplayername();
    toplayername_ = toplayername;
  } else {
    clear_has_toplayername();
    toplayername_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional int64 toPlayerOnlyId = 5;
inline bool Chat::has_toplayeronlyid() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void Chat::set_has_toplayeronlyid() {
  _has_bits_[0] |= 0x00000010u;
}
inline void Chat::clear_has_toplayeronlyid() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void Chat::clear_toplayeronlyid() {
  toplayeronlyid_ = GOOGLE_LONGLONG(0);
  clear_has_toplayeronlyid();
}
inline ::google::protobuf::int64 Chat::toplayeronlyid() const {
  return toplayeronlyid_;
}
inline void Chat::set_toplayeronlyid(::google::protobuf::int64 value) {
  set_has_toplayeronlyid();
  toplayeronlyid_ = value;
}

// optional string chatMsg = 6;
inline bool Chat::has_chatmsg() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void Chat::set_has_chatmsg() {
  _has_bits_[0] |= 0x00000020u;
}
inline void Chat::clear_has_chatmsg() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void Chat::clear_chatmsg() {
  if (chatmsg_ != &::google::protobuf::internal::kEmptyString) {
    chatmsg_->clear();
  }
  clear_has_chatmsg();
}
inline const ::std::string& Chat::chatmsg() const {
  return *chatmsg_;
}
inline void Chat::set_chatmsg(const ::std::string& value) {
  set_has_chatmsg();
  if (chatmsg_ == &::google::protobuf::internal::kEmptyString) {
    chatmsg_ = new ::std::string;
  }
  chatmsg_->assign(value);
}
inline void Chat::set_chatmsg(const char* value) {
  set_has_chatmsg();
  if (chatmsg_ == &::google::protobuf::internal::kEmptyString) {
    chatmsg_ = new ::std::string;
  }
  chatmsg_->assign(value);
}
inline void Chat::set_chatmsg(const char* value, size_t size) {
  set_has_chatmsg();
  if (chatmsg_ == &::google::protobuf::internal::kEmptyString) {
    chatmsg_ = new ::std::string;
  }
  chatmsg_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Chat::mutable_chatmsg() {
  set_has_chatmsg();
  if (chatmsg_ == &::google::protobuf::internal::kEmptyString) {
    chatmsg_ = new ::std::string;
  }
  return chatmsg_;
}
inline ::std::string* Chat::release_chatmsg() {
  clear_has_chatmsg();
  if (chatmsg_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = chatmsg_;
    chatmsg_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void Chat::set_allocated_chatmsg(::std::string* chatmsg) {
  if (chatmsg_ != &::google::protobuf::internal::kEmptyString) {
    delete chatmsg_;
  }
  if (chatmsg) {
    set_has_chatmsg();
    chatmsg_ = chatmsg;
  } else {
    clear_has_chatmsg();
    chatmsg_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional int32 vipLevel = 7;
inline bool Chat::has_viplevel() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void Chat::set_has_viplevel() {
  _has_bits_[0] |= 0x00000040u;
}
inline void Chat::clear_has_viplevel() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void Chat::clear_viplevel() {
  viplevel_ = 0;
  clear_has_viplevel();
}
inline ::google::protobuf::int32 Chat::viplevel() const {
  return viplevel_;
}
inline void Chat::set_viplevel(::google::protobuf::int32 value) {
  set_has_viplevel();
  viplevel_ = value;
}

// repeated .TimeInfo info = 8;
inline int Chat::info_size() const {
  return info_.size();
}
inline void Chat::clear_info() {
  info_.Clear();
}
inline const ::TimeInfo& Chat::info(int index) const {
  return info_.Get(index);
}
inline ::TimeInfo* Chat::mutable_info(int index) {
  return info_.Mutable(index);
}
inline ::TimeInfo* Chat::add_info() {
  return info_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::TimeInfo >&
Chat::info() const {
  return info_;
}
inline ::google::protobuf::RepeatedPtrField< ::TimeInfo >*
Chat::mutable_info() {
  return &info_;
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_chat_2eproto__INCLUDED
