/**
 * @file acceptor.h
 * @brief
 * @date 2022-02-27
 *
 * @copyright Copyright (c) 2022 InfraSail
 *
 */

#ifndef MRPROXYER_SRC_NETLIB_ACCEPTOR_H_
#define MRPROXYER_SRC_NETLIB_ACCEPTOR_H_

#include "../util/non_copyable_movable.h"
#include "io_event.h"
#include "log.h"
#include "net_address.h"
#include "netlib.h"
#include "socket.h"

namespace netlib {
class IoWatcher;

class Acceptor : util::NonCopyableMovable {
 public:
  Acceptor(IoWatcher* io_watcher, NetAddress bind_address);
  ~Acceptor();

  void SetOnAccept(const OnAcceptCallback& on_accept) {
    on_accept_ = on_accept;
  }

  void Listen();

 private:
  void OnAccept();

  Socket socket_;
  IoEvent event_;
  IoWatcher* io_watcher_;

  OnAcceptCallback on_accept_;
};
}  // namespace netlib
#endif  // MRPROXYER_SRC_NETLIB_ACCEPTOR_H_
