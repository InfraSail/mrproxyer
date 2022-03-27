/**
 * @file socket.cc
 * @brief
 * @date 2022-02-27
 *
 * @copyright Copyright (c) 2022 InfraSail
 *
 */

#include "socket.h"

// TODO chosed by os
#include <fcntl.h>

#include "../util/log.h"

using namespace netlib;

Socket::Socket(int domain, int type, int protocol) {
  int sock = ::socket(domain, type, protocol);
  if (sock < 0) {
    log_error("create socket error! errno=%d errstr = %s", errno,
              strerror(errno));
  }
  fd_ = sock;
}

Socket::Socket(int fd) : fd_(fd) {}

Socket::~Socket() {
  // close socket
  close(fd_);
}
void Socket::BindAddress(NetAddress& address) const {
  //  auto s = std::any_cast<sockaddr*>(address.GetAddress());
  //  auto size = address.GetSize();
  int ret = ::bind(fd_, std::any_cast<sockaddr*>(address.GetAddress()),
                   address.GetSize());
  if (ret < 0) {
    log_error("bind error! errno=%d errstr = %s", errno, strerror(errno));
  }
}

void Socket::Listen() const {
  int ret = ::listen(fd_, 1024);
  if (ret < 0) {
    log_error("listen error! errno=%d errstr = %s", errno, strerror(errno));
  }
}

int Socket::Accept(NetAddress* peer_address) const {
  socklen_t sock_len = peer_address->GetSize();
  int conn_fd = ::accept(
      fd_, std::any_cast<sockaddr*>(peer_address->GetAddress()), &sock_len);
  // TODO setnonblocking
  Socket::SetNonBlockAndCloseOnExec(conn_fd);
  // TODO Test write
  int opt = 3;
  if (::setsockopt(conn_fd, SOL_SOCKET, SO_SNDBUF, &opt,
                   (socklen_t)(sizeof(opt))) < 0) {
    log_error("SocketFd SetSO_SNDBUF error");
  }

  if (conn_fd < 0) {
    log_error("bind error! errno=%d errstr = %s", errno, strerror(errno));
  }
  return conn_fd;
}

// TODO socket ops
void Socket::SetTcpNoDelay(bool on) const {
  int opt = on ? 1 : 0;
  if (::setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &opt,
                   (socklen_t)(sizeof(opt))) < 0) {
    log_error("SocketFd SetReusePort error");
  }
}

void Socket::SetReuseAddress(bool on) const {
  int opt = on ? 1 : 0;
  if (::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt,
                   (socklen_t)(sizeof(opt))) < 0) {
    log_error("Socket::SetReuseAddress SetReuseAddress error");
  }
}

void Socket::SetReusePort(bool on) const {
  int opt = on ? 1 : 0;
  if (::setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &opt,
                   (socklen_t)(sizeof(opt))) < 0) {
    log_error("Socket::SetReuseAddress SetReusePort error");
  }
}

void Socket::SetKeepAlive(bool on) const {
  int opt = on ? 1 : 0;
  if (::setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, &opt,
                   (socklen_t)(sizeof(opt))) < 0) {
    log_error("Socket::SetKeepAlive SetKeepAlive error");
  }
}

ssize_t Socket::Read(void* buffer, size_t len) const {
  return ::read(fd_, buffer, len);
}
ssize_t Socket::Write(void* buffer, size_t len) const {
  return ::write(fd_, buffer, len);
}

void Socket::SetNonBlockAndCloseOnExec(int sock_fd) {
  // non-block
  int flags = ::fcntl(sock_fd, F_GETFL, 0);
  flags |= O_NONBLOCK;
  int ret = ::fcntl(sock_fd, F_SETFL, flags);
  // FIXME check

  // close-on-exec
  flags = ::fcntl(sock_fd, F_GETFD, 0);
  flags |= FD_CLOEXEC;
  ret = ::fcntl(sock_fd, F_SETFD, flags);
}
// TODO support ipv6 & udp now is ipv4 only
int Socket::CreateNonBlockFd(int domain, int type, int protocol) {
  int sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0) {
    log_error("create socket error! errno=%d errstr = %s", errno,
              strerror(errno));
  }
  return sock;
}
