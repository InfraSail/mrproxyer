/**
 * @file netlib.h
 * @brief
 * @date 2022-03-01
 *
 * @copyright Copyright (c) 2022 InfraSail
 *
 */

#ifndef MRPROXYER_SRC_NETLIB_NETLIB_H_
#define MRPROXYER_SRC_NETLIB_NETLIB_H_
// typedef and callbacks
#include <functional>
#include <memory>

#include "platform.h"

namespace netlib {

class IoEvent;
class IoBuf;
class TcpEvent;
class TimerManager;
class TimePoint;
class Socket;
class NetAddress;
class Timer;

// typedef
using TimerPrt_t = std::unique_ptr<Timer>;
using TcpEventPrt_t = std::shared_ptr<TcpEvent>;

using EventList = std::vector<IoEvent*>;
using ExpiredTimers = std::vector<TimerPrt_t>;
// callback
using EventCallback = std::function<void()>;
using TimerCallback = std::function<void()>;

using OnAcceptCallback =
    std::function<void(int conn_fd, NetAddress& peer_address)>;
using OnConnectionCallback = std::function<void(const TcpEventPrt_t& conn)>;
using OnMessageCallback =
    std::function<void(const TcpEventPrt_t& conn, IoBuf* buf)>;
using OnCloseCallback = std::function<void(const TcpEventPrt_t& conn)>;

#ifdef OS_UNIX
enum {
  TOHKA_NONE = 0x0000,
  TOHKA_READ = 0x0001,
  TOHKA_WRITE = 0x0004,
};

#endif

}  // namespace netlib

#endif  // MRPROXYER_SRC_NETLIB_NETLIB_H_
