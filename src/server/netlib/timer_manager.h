/**
 * @file timer_manager.h
 * @brief
 * @date 2022-02-20
 *
 * @copyright Copyright (c) 2022 InfraSail
 *
 */

#ifndef MRPROXYER_SRC_SERVER_NETLIB_TIMERMANAGER_H_
#define MRPROXYER_SRC_SERVER_NETLIB_TIMERMANAGER_H_

#include "netlib.h"
#include "timepoint.h"
#include "timer.h"

namespace netlib {
class TimerManager {
 public:
  static constexpr int64_t kDefaultTimeOutMs = 10000;

  void AddTimer(TimePoint when, TimerPrt_t tc);
  ExpiredTimers GetExpiredTimers();
  int64_t GetNextExpiredDuration();

 private:
  // for sort
  using TimerMap = std::multimap<TimePoint, TimerPrt_t>;
  TimerMap timer_map_;
};

}  // namespace netlib

#endif  // MRPROXYER_SRC_SERVER_NETLIB_TIMERMANAGER_H_
