/**
 * @file io_buf.cc
 * @brief
 * @date 2022-03-01
 *
 * @copyright Copyright (c) 2022 InfraSail
 *
 */

#include "io_buf.h"

using namespace netlib;

IoBuf::IoBuf(size_t len) : data_(len) {
  read_index_ = kPrependSize;
  write_index_ = kPrependSize;
}

void IoBuf::Append(const char* data, size_t len) {
  // get writeable space
  EnsureWritableBytes(len);
  //  std::copy()
  data_.insert(data_.begin() + (long)write_index_, data, data + len);
  assert(GetWriteableSize() >= len);
  write_index_ += len;
}
void IoBuf::Append(const void* data, size_t len) {
  Append((const char*)data, len);
}
void IoBuf::EnsureWritableBytes(size_t len) {
  if (GetWriteableSize() < len) {
    // re malloc size
    MakeSpace(len);
  }
}
void IoBuf::MakeSpace(size_t len) {
  // TODO  move readable data
  data_.resize(len + write_index_);
}
std::string IoBuf::ReceiveAllAsString() {
  size_t readable = GetReadableSize();

  // c++ 17
  std::string result{Peek(), 0, readable};

  // refresh
  Refresh();
  return result;
}
void IoBuf::Retrieve(size_t len) {
  assert(len <= GetReadableSize());
  if (len < GetReadableSize()) {
    read_index_ += len;
  } else {
    Refresh();
  }
}
void IoBuf::Refresh() {
  read_index_ = kPrependSize;
  write_index_ = kPrependSize;
}
