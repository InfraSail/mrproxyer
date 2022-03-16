/**
 * @file socket.h
 * @brief
 * @date 2022-02-27
 *
 * @copyright Copyright (c) 2022 InfraSail
 *
 */

#ifndef MRPROXYER_SRC_SERVER_NETLIB_SOCKET_H_
#define MRPROXYER_SRC_SERVER_NETLIB_SOCKET_H_

#include "../util/non_copyable_movable.h"
#include "net_address.h"
// socket and it's ops

namespace netlib {
class Socket : util::NonCopyableMovable {
 public:
  Socket() = default;
  Socket(int domain, int type, int protocol);
  explicit Socket(int fd);
  ~Socket();

  int GetFd() const { return fd_; }
  void BindAddress(NetAddress& address) const;
  void Listen() const;
  int Accept(NetAddress* peer_address) const;

  ssize_t Read(void* buffer, size_t len) const;
  ssize_t Write(void* buffer, size_t len) const;
  void SetTcpNoDelay(bool on) const;

  void SetReuseAddress(bool on) const;

  void SetReusePort(bool on) const;
  void SetKeepAlive(bool on) const;

  static void SetNonBlockAndCloseOnExec(int sock_fd);
  static int CreateNonBlockFd(int domain = AF_INET, int type = SOCK_STREAM,
                              int protocol = IPPROTO_TCP);

 private:
  int fd_;
};

}  // namespace netlib

#endif  // MRPROXYER_SRC_SERVER_NETLIB_SOCKET_H_
