// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: server.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "server.pb.h"

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

namespace {

const ::google::protobuf::Descriptor* ReqLoginServer_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ReqLoginServer_reflection_ = NULL;
const ::google::protobuf::Descriptor* AckLoginServer_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  AckLoginServer_reflection_ = NULL;
const ::google::protobuf::Descriptor* ServerHeartBeat_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ServerHeartBeat_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_server_2eproto() {
  protobuf_AddDesc_server_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "server.proto");
  GOOGLE_CHECK(file != NULL);
  ReqLoginServer_descriptor_ = file->message_type(0);
  static const int ReqLoginServer_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReqLoginServer, checkcode_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReqLoginServer, serverid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReqLoginServer, servertype_),
  };
  ReqLoginServer_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ReqLoginServer_descriptor_,
      ReqLoginServer::default_instance_,
      ReqLoginServer_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReqLoginServer, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReqLoginServer, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ReqLoginServer));
  AckLoginServer_descriptor_ = file->message_type(1);
  static const int AckLoginServer_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AckLoginServer, checkcode_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AckLoginServer, serverid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AckLoginServer, servertype_),
  };
  AckLoginServer_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      AckLoginServer_descriptor_,
      AckLoginServer::default_instance_,
      AckLoginServer_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AckLoginServer, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AckLoginServer, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(AckLoginServer));
  ServerHeartBeat_descriptor_ = file->message_type(2);
  static const int ServerHeartBeat_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ServerHeartBeat, isneedack_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ServerHeartBeat, checknum_),
  };
  ServerHeartBeat_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ServerHeartBeat_descriptor_,
      ServerHeartBeat::default_instance_,
      ServerHeartBeat_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ServerHeartBeat, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ServerHeartBeat, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ServerHeartBeat));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_server_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ReqLoginServer_descriptor_, &ReqLoginServer::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    AckLoginServer_descriptor_, &AckLoginServer::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ServerHeartBeat_descriptor_, &ServerHeartBeat::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_server_2eproto() {
  delete ReqLoginServer::default_instance_;
  delete ReqLoginServer_reflection_;
  delete AckLoginServer::default_instance_;
  delete AckLoginServer_reflection_;
  delete ServerHeartBeat::default_instance_;
  delete ServerHeartBeat_reflection_;
}

void protobuf_AddDesc_server_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\014server.proto\"I\n\016ReqLoginServer\022\021\n\tchec"
    "kCode\030\001 \001(\005\022\020\n\010serverId\030\002 \001(\005\022\022\n\nserverT"
    "ype\030\003 \001(\005\"I\n\016AckLoginServer\022\021\n\tcheckCode"
    "\030\001 \001(\005\022\020\n\010serverId\030\002 \001(\005\022\022\n\nserverType\030\003"
    " \001(\005\"6\n\017ServerHeartBeat\022\021\n\tisNeedAck\030\001 \001"
    "(\005\022\020\n\010checkNum\030\002 \001(\005B/\n\033com.sevencool.lo"
    "gic.netdataB\013ServerProtoZ\003ptf", 269);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "server.proto", &protobuf_RegisterTypes);
  ReqLoginServer::default_instance_ = new ReqLoginServer();
  AckLoginServer::default_instance_ = new AckLoginServer();
  ServerHeartBeat::default_instance_ = new ServerHeartBeat();
  ReqLoginServer::default_instance_->InitAsDefaultInstance();
  AckLoginServer::default_instance_->InitAsDefaultInstance();
  ServerHeartBeat::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_server_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_server_2eproto {
  StaticDescriptorInitializer_server_2eproto() {
    protobuf_AddDesc_server_2eproto();
  }
} static_descriptor_initializer_server_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int ReqLoginServer::kCheckCodeFieldNumber;
const int ReqLoginServer::kServerIdFieldNumber;
const int ReqLoginServer::kServerTypeFieldNumber;
#endif  // !_MSC_VER

ReqLoginServer::ReqLoginServer()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void ReqLoginServer::InitAsDefaultInstance() {
}

ReqLoginServer::ReqLoginServer(const ReqLoginServer& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void ReqLoginServer::SharedCtor() {
  _cached_size_ = 0;
  checkcode_ = 0;
  serverid_ = 0;
  servertype_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ReqLoginServer::~ReqLoginServer() {
  SharedDtor();
}

void ReqLoginServer::SharedDtor() {
  if (this != default_instance_) {
  }
}

void ReqLoginServer::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ReqLoginServer::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ReqLoginServer_descriptor_;
}

const ReqLoginServer& ReqLoginServer::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_server_2eproto();
  return *default_instance_;
}

ReqLoginServer* ReqLoginServer::default_instance_ = NULL;

ReqLoginServer* ReqLoginServer::New() const {
  return new ReqLoginServer;
}

void ReqLoginServer::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    checkcode_ = 0;
    serverid_ = 0;
    servertype_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ReqLoginServer::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int32 checkCode = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &checkcode_)));
          set_has_checkcode();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_serverId;
        break;
      }

      // optional int32 serverId = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_serverId:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &serverid_)));
          set_has_serverid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_serverType;
        break;
      }

      // optional int32 serverType = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_serverType:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &servertype_)));
          set_has_servertype();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void ReqLoginServer::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional int32 checkCode = 1;
  if (has_checkcode()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->checkcode(), output);
  }

  // optional int32 serverId = 2;
  if (has_serverid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->serverid(), output);
  }

  // optional int32 serverType = 3;
  if (has_servertype()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->servertype(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* ReqLoginServer::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional int32 checkCode = 1;
  if (has_checkcode()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->checkcode(), target);
  }

  // optional int32 serverId = 2;
  if (has_serverid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->serverid(), target);
  }

  // optional int32 serverType = 3;
  if (has_servertype()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->servertype(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int ReqLoginServer::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int32 checkCode = 1;
    if (has_checkcode()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->checkcode());
    }

    // optional int32 serverId = 2;
    if (has_serverid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->serverid());
    }

    // optional int32 serverType = 3;
    if (has_servertype()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->servertype());
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

void ReqLoginServer::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ReqLoginServer* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ReqLoginServer*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ReqLoginServer::MergeFrom(const ReqLoginServer& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_checkcode()) {
      set_checkcode(from.checkcode());
    }
    if (from.has_serverid()) {
      set_serverid(from.serverid());
    }
    if (from.has_servertype()) {
      set_servertype(from.servertype());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ReqLoginServer::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ReqLoginServer::CopyFrom(const ReqLoginServer& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ReqLoginServer::IsInitialized() const {

  return true;
}

void ReqLoginServer::Swap(ReqLoginServer* other) {
  if (other != this) {
    std::swap(checkcode_, other->checkcode_);
    std::swap(serverid_, other->serverid_);
    std::swap(servertype_, other->servertype_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ReqLoginServer::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ReqLoginServer_descriptor_;
  metadata.reflection = ReqLoginServer_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int AckLoginServer::kCheckCodeFieldNumber;
const int AckLoginServer::kServerIdFieldNumber;
const int AckLoginServer::kServerTypeFieldNumber;
#endif  // !_MSC_VER

AckLoginServer::AckLoginServer()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void AckLoginServer::InitAsDefaultInstance() {
}

AckLoginServer::AckLoginServer(const AckLoginServer& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void AckLoginServer::SharedCtor() {
  _cached_size_ = 0;
  checkcode_ = 0;
  serverid_ = 0;
  servertype_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

AckLoginServer::~AckLoginServer() {
  SharedDtor();
}

void AckLoginServer::SharedDtor() {
  if (this != default_instance_) {
  }
}

void AckLoginServer::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* AckLoginServer::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return AckLoginServer_descriptor_;
}

const AckLoginServer& AckLoginServer::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_server_2eproto();
  return *default_instance_;
}

AckLoginServer* AckLoginServer::default_instance_ = NULL;

AckLoginServer* AckLoginServer::New() const {
  return new AckLoginServer;
}

void AckLoginServer::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    checkcode_ = 0;
    serverid_ = 0;
    servertype_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool AckLoginServer::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int32 checkCode = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &checkcode_)));
          set_has_checkcode();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_serverId;
        break;
      }

      // optional int32 serverId = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_serverId:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &serverid_)));
          set_has_serverid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_serverType;
        break;
      }

      // optional int32 serverType = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_serverType:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &servertype_)));
          set_has_servertype();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void AckLoginServer::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional int32 checkCode = 1;
  if (has_checkcode()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->checkcode(), output);
  }

  // optional int32 serverId = 2;
  if (has_serverid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->serverid(), output);
  }

  // optional int32 serverType = 3;
  if (has_servertype()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->servertype(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* AckLoginServer::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional int32 checkCode = 1;
  if (has_checkcode()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->checkcode(), target);
  }

  // optional int32 serverId = 2;
  if (has_serverid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->serverid(), target);
  }

  // optional int32 serverType = 3;
  if (has_servertype()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->servertype(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int AckLoginServer::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int32 checkCode = 1;
    if (has_checkcode()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->checkcode());
    }

    // optional int32 serverId = 2;
    if (has_serverid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->serverid());
    }

    // optional int32 serverType = 3;
    if (has_servertype()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->servertype());
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

void AckLoginServer::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const AckLoginServer* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const AckLoginServer*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void AckLoginServer::MergeFrom(const AckLoginServer& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_checkcode()) {
      set_checkcode(from.checkcode());
    }
    if (from.has_serverid()) {
      set_serverid(from.serverid());
    }
    if (from.has_servertype()) {
      set_servertype(from.servertype());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void AckLoginServer::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void AckLoginServer::CopyFrom(const AckLoginServer& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool AckLoginServer::IsInitialized() const {

  return true;
}

void AckLoginServer::Swap(AckLoginServer* other) {
  if (other != this) {
    std::swap(checkcode_, other->checkcode_);
    std::swap(serverid_, other->serverid_);
    std::swap(servertype_, other->servertype_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata AckLoginServer::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = AckLoginServer_descriptor_;
  metadata.reflection = AckLoginServer_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int ServerHeartBeat::kIsNeedAckFieldNumber;
const int ServerHeartBeat::kCheckNumFieldNumber;
#endif  // !_MSC_VER

ServerHeartBeat::ServerHeartBeat()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void ServerHeartBeat::InitAsDefaultInstance() {
}

ServerHeartBeat::ServerHeartBeat(const ServerHeartBeat& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void ServerHeartBeat::SharedCtor() {
  _cached_size_ = 0;
  isneedack_ = 0;
  checknum_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ServerHeartBeat::~ServerHeartBeat() {
  SharedDtor();
}

void ServerHeartBeat::SharedDtor() {
  if (this != default_instance_) {
  }
}

void ServerHeartBeat::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ServerHeartBeat::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ServerHeartBeat_descriptor_;
}

const ServerHeartBeat& ServerHeartBeat::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_server_2eproto();
  return *default_instance_;
}

ServerHeartBeat* ServerHeartBeat::default_instance_ = NULL;

ServerHeartBeat* ServerHeartBeat::New() const {
  return new ServerHeartBeat;
}

void ServerHeartBeat::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    isneedack_ = 0;
    checknum_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ServerHeartBeat::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int32 isNeedAck = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &isneedack_)));
          set_has_isneedack();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_checkNum;
        break;
      }

      // optional int32 checkNum = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_checkNum:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &checknum_)));
          set_has_checknum();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void ServerHeartBeat::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional int32 isNeedAck = 1;
  if (has_isneedack()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->isneedack(), output);
  }

  // optional int32 checkNum = 2;
  if (has_checknum()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->checknum(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* ServerHeartBeat::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional int32 isNeedAck = 1;
  if (has_isneedack()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->isneedack(), target);
  }

  // optional int32 checkNum = 2;
  if (has_checknum()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->checknum(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int ServerHeartBeat::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int32 isNeedAck = 1;
    if (has_isneedack()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->isneedack());
    }

    // optional int32 checkNum = 2;
    if (has_checknum()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->checknum());
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

void ServerHeartBeat::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ServerHeartBeat* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ServerHeartBeat*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ServerHeartBeat::MergeFrom(const ServerHeartBeat& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_isneedack()) {
      set_isneedack(from.isneedack());
    }
    if (from.has_checknum()) {
      set_checknum(from.checknum());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ServerHeartBeat::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ServerHeartBeat::CopyFrom(const ServerHeartBeat& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ServerHeartBeat::IsInitialized() const {

  return true;
}

void ServerHeartBeat::Swap(ServerHeartBeat* other) {
  if (other != this) {
    std::swap(isneedack_, other->isneedack_);
    std::swap(checknum_, other->checknum_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ServerHeartBeat::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ServerHeartBeat_descriptor_;
  metadata.reflection = ServerHeartBeat_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
