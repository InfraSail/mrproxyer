/**
 * @file io_event.h
 * @brief
 * @date 2022-02-20
 *
 * @copyright Copyright (c) 2022 InfraSail
 *
 */

#ifndef MRPROXYER_SRC_NETLIB_IO_EVENT_H_
#define MRPROXYER_SRC_NETLIB_IO_EVENT_H_

#include "../util/non_copyable_movable.h"
#include "io_watcher.h"
#include "netlib.h"

namespace netlib {

class IoEvent : util::NonCopyableMovable {
 public:
  IoEvent(IoWatcher* io_watcher, int fd);

  // Register to the monitor event of Poll
  void Register();

  // Unregister to the monitor event of Poll
  void UnRegister();

  void ExecuteEvent();

  void EnableReading() {
    //    events_ |= POLL_IN | POLL_PRI;
    events_ |= TOHKA_READ;
    Register();
  };
  void DisableReading() {
    events_ &= ~TOHKA_READ;
    Register();
  };
  void EnableWriting() {
    events_ |= TOHKA_WRITE;
    Register();
  };
  void DisableWriting() {
    events_ &= ~TOHKA_WRITE;
    Register();
  };
  void DisableAll() {
    events_ = TOHKA_NONE;
    Register();
  };
  void SetReadCallback(EventCallback read_callback) {
    read_callback_ = std::move(read_callback);
  }
  void SetWriteCallback(EventCallback write_callback) {
    write_callback_ = std::move(write_callback);
  }
  void SetCloseCallback(EventCallback close_callback) {
    close_callback_ = std::move(close_callback);
  }
  void SetErrorCallback(EventCallback error_callback) {
    error_callback_ = std::move(error_callback);
  }
  short GetEvents() const { return events_; }
  void SetEvents(short events) { events_ = events; }
  short GetRevents() const { return revents_; }
  void SetRevents(short revents) { revents_ = revents; }
  int GetFd() const { return fd_; }
  int GetIndex() const { return index_; }
  void SetIndex(int index) { index_ = index; }

 private:
  int fd_;
  short events_;
  short revents_;
  int index_;  // for poller

  IoWatcher* io_watcher_;
  EventCallback read_callback_;
  EventCallback write_callback_;
  EventCallback close_callback_;
  EventCallback error_callback_;
};

}  // namespace netlib

#endif  // MRPROXYER_SRC_NETLIB_IO_EVENT_H_
