/**
 * @file timepoint.h
 * @brief
 * @date 2022-02-20
 *
 * @copyright Copyright (c) 2022 InfraSail
 *
 */

#ifndef MRPROXYER_SRC_SERVER_NETLIB_TIMEPOINT_H_
#define MRPROXYER_SRC_SERVER_NETLIB_TIMEPOINT_H_

#include "platform.h"

namespace netlib {
class TimePoint {
 public:
  // init an invalid time
  TimePoint() : microseconds_(-1) {}
  explicit TimePoint(int64_t microseconds) : microseconds_(microseconds) {}

  int64_t GetMicroSeconds() const { return microseconds_; }
  int64_t GetMilliSeconds() const {
    return microseconds_ / kMilliSecondsPerSecond;
  }
  std::string ToString() const;
  std::string ToFormatString(bool show_microseconds = true) const;
  static TimePoint now();
  bool operator<(const TimePoint& other) const {
    return this->microseconds_ < other.microseconds_;
  }
  bool operator==(const TimePoint& other) const {
    return this->microseconds_ == other.microseconds_;
  }
  TimePoint operator+(int32_t delay_ms) const {
    return TimePoint{microseconds_ + delay_ms * kMilliSecondsPerSecond};
  }

  static constexpr int kMicroSecondPerSecond = 1000 * 1000;
  static constexpr int kMilliSecondsPerSecond = 1000;

 private:
  // microseconds since epoch
  int64_t microseconds_;
};

}  // namespace netlib
#endif  // MRPROXYER_SRC_SERVER_NETLIB_TIMEPOINT_H_
