/**
 * @file io_watcher.h
 * @brief
 * @date 2022-02-24
 *
 * @copyright Copyright (c) 2022 InfraSail
 *
 */

#ifndef MRPROXYER_SRC_NETLIB_IOWATCHER_H_
#define MRPROXYER_SRC_NETLIB_IOWATCHER_H_

#include "../util/non_copyable_movable.h"
#include "netlib.h"
#include "timepoint.h"

namespace netlib {

class IoEvent;
class IoWatcher : util::NonCopyableMovable {
 public:
  virtual ~IoWatcher() = default;
  // call poll
  virtual TimePoint PollEvents(int timeout, EventList* event_list) = 0;
  // Register and Update io_event to io_events_map
  virtual void RegisterEvent(IoEvent* io_event) = 0;
  virtual void UnRegisterEvent(IoEvent* io_event) = 0;

 protected:
  // map(fd->io_event)
  using IoEventsMap = std::map<int, IoEvent*>;
  IoEventsMap io_events_map;
};

}  // namespace netlib

#endif  // MRPROXYER_SRC_NETLIB_IOWATCHER_H_
