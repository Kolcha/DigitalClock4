/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2020  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "mouse_tracker.h"

#import <AppKit/NSEvent.h>
#import <AppKit/NSScreen.h>

namespace digital_clock {

class MouseTrackerPrivate
{
public:
  id global_monitor_ = nil;
  id local_monitor_ = nil;
};


static NSPoint point_from_top_left(NSPoint p)
{
  CGFloat global_height = 0;
  for (NSScreen* screen in [NSScreen screens]) {
    NSRect rect = [screen frame];
    CGFloat cur_height = rect.origin.y + rect.size.height;
    if (cur_height > global_height)
      global_height = cur_height;
  }

  p.y = global_height - p.y;
  return p;
}


MouseTracker::MouseTracker(QObject* parent)
  : QObject(parent)
  , impl_(new MouseTrackerPrivate())
{
}

MouseTracker::~MouseTracker()
{
  delete impl_;
}

void MouseTracker::start()
{
  void (^mouse_even_handler)(NSEvent*) = ^(NSEvent* event) {
    if ([event type] == NSEventTypeMouseMoved) {
      NSPoint p = point_from_top_left([NSEvent mouseLocation]);
      emit mousePositionChanged(QPoint(p.x, p.y));
    }
  };

  if (!impl_->global_monitor_) {
    impl_->global_monitor_ = [NSEvent addGlobalMonitorForEventsMatchingMask:
            (NSEventMaskMouseMoved)
            handler:mouse_even_handler];
  }

  if (!impl_->local_monitor_) {
    impl_->local_monitor_ = [NSEvent addLocalMonitorForEventsMatchingMask:
            (NSEventMaskMouseMoved)
            handler:^(NSEvent* event) {
        mouse_even_handler(event);
        return event;
    }];
  }
}

void MouseTracker::stop()
{
  if (impl_->global_monitor_) {
    [NSEvent removeMonitor: impl_->global_monitor_];
    impl_->global_monitor_ = nil;
  }

  if (impl_->local_monitor_) {
    [NSEvent removeMonitor: impl_->local_monitor_];
    impl_->local_monitor_ = nil;
  }
}

} // namespace digital_clock
