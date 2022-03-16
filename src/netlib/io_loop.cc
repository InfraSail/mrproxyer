/**
 * @file io_loop.cc
 * @brief
 * @date 2022-02-17
 *
 * @copyright Copyright (c) 2022 InfraSail
 *
 */

#include "io_loop.h"

#include "log.h"

using namespace netlib;

IoLoop::IoLoop()
    : running_(false),
      io_watcher_(std::make_unique<Poll>()),  // TODO choose poller by os
      timer_manager_(std::make_unique<TimerManager>()) {
  // init log level
  //  log_set_level(LOG_INFO);
  //  log_set_level(LOG_INFO);
}

void IoLoop::RunForever() {
  running_ = true;
  std::vector<IoEvent*> activate_event_list;
  int i = 10000000;
  while (i--) {
    activate_event_list.clear();
    int64_t next_expired_duration = timer_manager_->GetNextExpiredDuration();

    // get activate event and fill those to activate_event_list
    io_watcher_->PollEvents((int)next_expired_duration, &activate_event_list);

    // do io event
    for (auto event : activate_event_list) {
      event->ExecuteEvent();
    }
    // do timer
    for (auto& timer : timer_manager_->GetExpiredTimers()) {
      timer->run();
      if (timer->IsRepeat()) {
        const auto when = timer->GetExpiredTime() + (int)timer->GetDelay();
        timer_manager_->AddTimer(when, std::move(timer));
      }
      // timer delete there
    }
  }
}
void IoLoop::CallAt(TimePoint when, IoLoop::TimerTask callback) {
  auto timer = std::make_unique<Timer>(when, std::move(callback));
  timer_manager_->AddTimer(when, move(timer));
}
void IoLoop::CallLater(int delay, IoLoop::TimerTask callback) {
  auto now = TimePoint::now();
  CallAt(now + delay, std::move(callback));
}
void IoLoop::CallEvery(int interval, IoLoop::TimerTask callback) {
  auto when = TimePoint::now() + interval;
  auto timer =
      std::make_unique<Timer>(when, std::move(callback), interval, true);
  timer_manager_->AddTimer(when, std::move(timer));
}
IoWatcher* IoLoop::GetPoint() { return io_watcher_.get(); }
