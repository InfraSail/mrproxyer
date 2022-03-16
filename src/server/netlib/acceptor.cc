/**
 * @file acceptor.cc
 * @brief
 * @date 2022-02-27
 *
 * @copyright Copyright (c) 2022 InfraSail
 *
 */

#include "acceptor.h"

using namespace netlib;

Acceptor::Acceptor(IoWatcher* io_watcher, NetAddress bind_address)
    : io_watcher_(io_watcher),
      socket_(Socket::CreateNonBlockFd(AF_INET, SOCK_STREAM, IPPROTO_TCP)),
      event_(io_watcher, socket_.GetFd()) {
  // ipv4 or ipv6
  socket_.SetReusePort(true);
  socket_.BindAddress(bind_address);

  // set read callback
  event_.SetReadCallback([this] { OnAccept(); });
  event_.EnableReading();
};

void Acceptor::OnAccept() {
  NetAddress peer_address;
  int conn_fd = socket_.Accept(&peer_address);
  if (conn_fd > 0) {
    if (on_accept_) {
      on_accept_(conn_fd, peer_address);
    } else {
      log_warn("no OnAccept callback!");
    }
  } else {
    // TODO handle error
    log_error("accept error!");
  }
}
Acceptor::~Acceptor() {
  event_.DisableAll();
  event_.UnRegister();
}
void Acceptor::Listen() { socket_.Listen(); }
