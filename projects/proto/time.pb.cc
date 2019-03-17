// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: time.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "time.pb.h"

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

const ::google::protobuf::Descriptor* TimeInfo_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  TimeInfo_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_time_2eproto() {
  protobuf_AddDesc_time_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "time.proto");
  GOOGLE_CHECK(file != NULL);
  TimeInfo_descriptor_ = file->message_type(0);
  static const int TimeInfo_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TimeInfo, tag_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TimeInfo, sendtime_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TimeInfo, receivetime_),
  };
  TimeInfo_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      TimeInfo_descriptor_,
      TimeInfo::default_instance_,
      TimeInfo_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TimeInfo, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TimeInfo, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(TimeInfo));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_time_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    TimeInfo_descriptor_, &TimeInfo::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_time_2eproto() {
  delete TimeInfo::default_instance_;
  delete TimeInfo_reflection_;
}

void protobuf_AddDesc_time_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\ntime.proto\">\n\010TimeInfo\022\013\n\003tag\030\001 \001(\t\022\020\n"
    "\010sendTime\030\002 \001(\003\022\023\n\013receiveTime\030\003 \001(\003B-\n\033"
    "com.sevencool.logic.netdataB\tTimeProtoZ\003"
    "ptf", 123);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "time.proto", &protobuf_RegisterTypes);
  TimeInfo::default_instance_ = new TimeInfo();
  TimeInfo::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_time_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_time_2eproto {
  StaticDescriptorInitializer_time_2eproto() {
    protobuf_AddDesc_time_2eproto();
  }
} static_descriptor_initializer_time_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int TimeInfo::kTagFieldNumber;
const int TimeInfo::kSendTimeFieldNumber;
const int TimeInfo::kReceiveTimeFieldNumber;
#endif  // !_MSC_VER

TimeInfo::TimeInfo()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void TimeInfo::InitAsDefaultInstance() {
}

TimeInfo::TimeInfo(const TimeInfo& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void TimeInfo::SharedCtor() {
  _cached_size_ = 0;
  tag_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  sendtime_ = GOOGLE_LONGLONG(0);
  receivetime_ = GOOGLE_LONGLONG(0);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

TimeInfo::~TimeInfo() {
  SharedDtor();
}

void TimeInfo::SharedDtor() {
  if (tag_ != &::google::protobuf::internal::kEmptyString) {
    delete tag_;
  }
  if (this != default_instance_) {
  }
}

void TimeInfo::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* TimeInfo::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return TimeInfo_descriptor_;
}

const TimeInfo& TimeInfo::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_time_2eproto();
  return *default_instance_;
}

TimeInfo* TimeInfo::default_instance_ = NULL;

TimeInfo* TimeInfo::New() const {
  return new TimeInfo;
}

void TimeInfo::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_tag()) {
      if (tag_ != &::google::protobuf::internal::kEmptyString) {
        tag_->clear();
      }
    }
    sendtime_ = GOOGLE_LONGLONG(0);
    receivetime_ = GOOGLE_LONGLONG(0);
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool TimeInfo::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional string tag = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_tag()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->tag().data(), this->tag().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_sendTime;
        break;
      }

      // optional int64 sendTime = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_sendTime:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &sendtime_)));
          set_has_sendtime();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_receiveTime;
        break;
      }

      // optional int64 receiveTime = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_receiveTime:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &receivetime_)));
          set_has_receivetime();
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

void TimeInfo::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional string tag = 1;
  if (has_tag()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->tag().data(), this->tag().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->tag(), output);
  }

  // optional int64 sendTime = 2;
  if (has_sendtime()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(2, this->sendtime(), output);
  }

  // optional int64 receiveTime = 3;
  if (has_receivetime()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(3, this->receivetime(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* TimeInfo::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional string tag = 1;
  if (has_tag()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->tag().data(), this->tag().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->tag(), target);
  }

  // optional int64 sendTime = 2;
  if (has_sendtime()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(2, this->sendtime(), target);
  }

  // optional int64 receiveTime = 3;
  if (has_receivetime()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(3, this->receivetime(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int TimeInfo::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional string tag = 1;
    if (has_tag()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->tag());
    }

    // optional int64 sendTime = 2;
    if (has_sendtime()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->sendtime());
    }

    // optional int64 receiveTime = 3;
    if (has_receivetime()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->receivetime());
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

void TimeInfo::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const TimeInfo* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const TimeInfo*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void TimeInfo::MergeFrom(const TimeInfo& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_tag()) {
      set_tag(from.tag());
    }
    if (from.has_sendtime()) {
      set_sendtime(from.sendtime());
    }
    if (from.has_receivetime()) {
      set_receivetime(from.receivetime());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void TimeInfo::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void TimeInfo::CopyFrom(const TimeInfo& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool TimeInfo::IsInitialized() const {

  return true;
}

void TimeInfo::Swap(TimeInfo* other) {
  if (other != this) {
    std::swap(tag_, other->tag_);
    std::swap(sendtime_, other->sendtime_);
    std::swap(receivetime_, other->receivetime_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata TimeInfo::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = TimeInfo_descriptor_;
  metadata.reflection = TimeInfo_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
