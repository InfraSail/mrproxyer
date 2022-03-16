/**
 * @file tcp_server.h
 * @brief
 * @date 2022-03-02
 *
 * @copyright Copyright (c) 2022 InfraSail
 *
 */

#ifndef MRPROXYER_SRC_NETLIB_TCPSERVER_H_
#define MRPROXYER_SRC_NETLIB_TCPSERVER_H_

#include "acceptor.h"
#include "io_watcher.h"
#include "log.h"
#include "netlib.h"
#include "non_copyable_movable.h"
#include "tcp_event.h"

// manage tcpevent
namespace netlib {
class TcpServer : NonCopyableMovable {
 public:
  TcpServer(IoWatcher* io_watcher, NetAddress& bind_address);
  ~TcpServer();

  void Run();
  void SetOnConnection(const OnConnectionCallback& cb) { on_connection_ = cb; }
  void SetOnMessage(const OnMessageCallback& cb) { on_message_ = cb; }

 private:
  // call OnConnectionCallback
  void OnAccept(int conn_fd, NetAddress& peer_address);

  void OnClose(const TcpEventPrt_t& conn);

  IoWatcher* io_watcher_;
  std::unique_ptr<Acceptor> acceptor_;
  std::map<std::string, TcpEventPrt_t> connection_map_;
  OnConnectionCallback on_connection_;
  OnMessageCallback on_message_;
};
}  // namespace netlib

#endif  // MRPROXYER_SRC_NETLIB_TCPSERVER_H_
