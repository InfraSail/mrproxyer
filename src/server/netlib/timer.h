/**
 * @file timer.h
 * @brief
 * @date 2022-02-23
 *
 * @copyright Copyright (c) 2022 InfraSail
 *
 */

#ifndef MRPROXYER_SRC_SERVER_NETLIB_TIMER_H_
#define MRPROXYER_SRC_SERVER_NETLIB_TIMER_H_

#include "netlib.h"
#include "timepoint.h"

namespace netlib {

class Timer {
 public:
  Timer() = delete;
  Timer(TimePoint expired_time, TimerCallback timer_callback)
      : expired_time_(expired_time),
        timer_callback_(std::move(timer_callback)){};
  Timer(TimePoint expired_time, TimerCallback timer_callback, int64_t delay,
        bool repeat = false)
      : expired_time_(expired_time),
        timer_callback_(std::move(timer_callback)),
        delay_{delay},
        repeat_{repeat} {};
  void run() { timer_callback_(); }

  TimePoint GetExpiredTime() const { return expired_time_; }
  void SetExpiredTime(const TimePoint ExpiredTime) {
    expired_time_ = ExpiredTime;
  }
  bool IsRepeat() const { return repeat_; }
  void SetRepeat(bool Repeat) { repeat_ = Repeat; }
  int64_t GetDelay() const { return delay_; }
  void SetDelay(int64_t Delay) { delay_ = Delay; }

 private:
  TimePoint expired_time_;
  TimerCallback timer_callback_;
  bool repeat_{false};
  int64_t delay_{-1};  // ms
};

}  // namespace netlib

#endif  // MRPROXYER_SRC_SERVER_NETLIB_TIMER_H_
