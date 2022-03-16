/**
 * @file timepoint.cc
 * @brief
 * @date 2022-02-20
 *
 * @copyright Copyright (c) 2022 InfraSail
 *
 */

#include "timepoint.h"

using namespace netlib;

TimePoint TimePoint::now() {
  auto duration = std::chrono::system_clock::now().time_since_epoch();
  auto micro_seconds =
      std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
  return TimePoint(micro_seconds);
}
std::string TimePoint::ToFormatString(bool show_microseconds) const {
  auto time_t = (std::time_t)(microseconds_ / kMicroSecondPerSecond);
  auto tm = std::localtime(&time_t);
  char time_buffer[64]{};
  if (show_microseconds) {
    auto microseconds = microseconds_ % kMicroSecondPerSecond;
    snprintf(time_buffer, sizeof(time_buffer),
             "%d%02d%02d %02d:%02d:%02d:%06ld", tm->tm_year + 1900,
             tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec,
             microseconds);
  } else {
    snprintf(time_buffer, sizeof(time_buffer), "%d%02d%02d %02d:%02d:%02d",
             tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour,
             tm->tm_min, tm->tm_sec);
  }

  return {time_buffer};
}
std::string TimePoint::ToString() const {
  return std::to_string(microseconds_);
}
