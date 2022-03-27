/**
 * @file tcp_event.cc
 * @brief
 * @date 2022-03-01
 *
 * @copyright Copyright (c) 2022 InfraSail
 *
 */

#include "tcp_event.h"

using namespace netlib;

TcpEvent::TcpEvent(IoWatcher* io_watcher, std::string name, int fd,
                   NetAddress& peer)
    : io_watcher_(io_watcher),
      name_(std::move(name)),
      state_(kConnecting),
      socket_(std::make_unique<Socket>(fd)),
      event_(std::make_unique<IoEvent>(io_watcher, fd)),
      peer_(peer) {
  socket_->SetKeepAlive(true);

  event_->SetReadCallback([this] { HandleRead(); });
  event_->SetWriteCallback([this] { HandleWrite(); });
  event_->SetCloseCallback([this] { HandleClose(); });
  event_->SetErrorCallback([this] { HandleError(); });
}

void TcpEvent::HandleRead() {
  log_trace("TcpEvent::HandleRead fd = %d", socket_->GetFd());
  // read(event.GetFd())
  // TODO should add read fd?
  char buffer[64 * 1024];
  ssize_t n = socket_->Read(buffer, 64 * 1024);  // read from fd

  if (n > 0) {
    in_buf_.Append(buffer, n);
    // call msg callback
    on_message_(shared_from_this(), &in_buf_);
  } else if (n == 0) {
    log_trace("TcpEvent::HandleRead half close", socket_->GetFd());
    HandleClose();
  } else {
    HandleError();
  }
}
void TcpEvent::HandleWrite() {
  log_trace("TcpEvent::HandleWrite");
  ssize_t n =
      socket_->Write((char*)out_buf_.Peek(), out_buf_.GetReadableSize());
  log_trace("write %d bytes to socket fd %d", n, socket_->GetFd());
  if (n > 0) {
    out_buf_.Retrieve(n);
    // Once the data is written, close the write event immediately to avoid busy
    // loop
    if (out_buf_.GetReadableSize() == 0) {
      // TODO  call write done callback
      StopWriting();
    }
  } else {
    log_error("TcpEvent::HandleWrite error");
  }
}
void TcpEvent::HandleClose() {
  assert(state_ == kConnected || state_ == kDisconnecting);
  event_->DisableAll();
  if (on_close_) {
    //  close_callback();
    on_close_(shared_from_this());
  }
}
void TcpEvent::HandleError() { log_error("TcpEvent::HandleError!"); }
TcpEvent::~TcpEvent() {
  assert(state_ == kDisconnected);
  log_trace("TcpEvent::~TcpEvent");
}

void TcpEvent::OnEstablishing() {
  assert(state_ == kConnecting);
  SetState(kConnected);
  StartReading();

  // on connection open(accepted callback)
  if (on_connection_) {
    on_connection_(shared_from_this());
  }
}
void TcpEvent::OnDestroying() {
  if (state_ == kConnected) {
    SetState(kDisconnected);
    // TODO FIXME:why TcpEvent::HandleClose() call event_->DisableAll();
    event_->DisableAll();
    // on connection close(closed callback)
    if (on_connection_) {
      on_connection_(shared_from_this());
    }
  }
  // remove event from event map and  remove fd from pfds
  event_->UnRegister();
}
void TcpEvent::Send(std::string msg) { Send(msg.data(), msg.length()); }
void TcpEvent::Send(const char* data, size_t len) {
  if (state_ == kDisconnected) {
    log_warn("disconnected, give up writing");
    return;
  }
  size_t remaining = len;
  ssize_t n = 0;
  // If there is still data in the output buffer at this time,
  // it should not be sent directly, but the data is added to the buffer.
  if (out_buf_.GetReadableSize() == 0) {
    n = socket_->Write((char*)data, len);
    if (n >= 0) {
      // may be not write done
      remaining -= n;
      if (remaining == 0) {
        // FIXME write done callback
      }
    } else {
      if (errno != EWOULDBLOCK) {
        log_error("TcpEvent::Send errno != EWOULDBLOCK");
      }
    }
  }

  // Put the unsent data into the output buffer and pay attention to the write
  // event
  if (remaining > 0) {
    // append remaining data to buffer
    out_buf_.Append(data + n, remaining);
    log_trace("TcpEvent::Send no more buffer enable writing...");
    StartWriting();
  }
}
