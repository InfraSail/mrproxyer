/**
 * @file io_loop.h
 * @brief
 * @date 2022-02-17
 *
 * @copyright Copyright (c) 2022 InfraSail
 *
 */

#ifndef MRPROXYER_SRC_SERVER_NETLIB_IO_LOOP_H_
#define MRPROXYER_SRC_SERVER_NETLIB_IO_LOOP_H_

#include "io_event.h"
#include "io_watcher.h"
#include "platform.h"
#include "poll.h"
#include "timepoint.h"
#include "timer_manager.h"

namespace netlib {

class IoLoop : public util::NonCopyableMovable {
 public:
  using TimerTask = std::function<void()>;
  IoLoop();

  ~IoLoop() = default;
  void RunForever();
  void Quit() { running_ = false; };

  //  void CallSoon();
  void CallAt(TimePoint when, TimerTask callback);
  void CallLater(int delay, TimerTask callback);
  void CallEvery(int interval, TimerTask callback);

  IoWatcher* GetPoint();

 private:
  using IoWatcherPtr = std::unique_ptr<IoWatcher>;
  using TimerManagerPtr = std::unique_ptr<TimerManager>;
  IoWatcherPtr io_watcher_;
  TimerManagerPtr timer_manager_;

  bool running_;
};

}  // namespace netlib

#endif  // MRPROXYER_SRC_SERVER_NETLIB_IO_LOOP_H_
