/**
 * @file poll.h
 * @brief
 * @date 2022-02-24
 *
 * @copyright Copyright (c) 2022 InfraSail
 *
 */

#ifndef MRPROXYER_SRC_NETLIB_POLL_H_
#define MRPROXYER_SRC_NETLIB_POLL_H_

#include "io_watcher.h"
#include "non_copyable_movable.h"
#include "platform.h"

#ifdef OS_UNIX
#include "sys/poll.h"
#endif

namespace netlib {
class Poll : public IoWatcher {
 public:
  Poll();
  TimePoint PollEvents(int timeout, EventList* event_list) override;
  void RegisterEvent(IoEvent* io_event) override;
  void UnRegisterEvent(IoEvent* io_event) override;

 private:
  static constexpr int kInitialSize = 64;
  using Pfds = std::vector<struct pollfd>;
  Pfds pfds_;
};

}  // namespace netlib

#endif  // MRPROXYER_SRC_NETLIB_POLL_H_
