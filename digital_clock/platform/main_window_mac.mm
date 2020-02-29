/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2017-2020  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "gui/clock_window.h"

#import <AppKit/NSWindow.h>

namespace digital_clock {
namespace gui {

void ClockWindow::SetVisibleOnAllDesktops(bool set)
{
  // original solution was found at
  // http://stackoverflow.com/questions/16775352/keep-a-application-window-always-on-current-desktop-on-linux-and-mac/
  // but was rewritten to Objective C++ due to macOS 10.15 SDK changes
  NSView* view = reinterpret_cast<NSView*>(winId());
  NSWindow* window = [view window];
  NSWindowCollectionBehavior behavior = NSWindowCollectionBehaviorDefault;
  if (set)
    behavior = NSWindowCollectionBehaviorCanJoinAllSpaces | NSWindowCollectionBehaviorFullScreenNone;
  [window setCollectionBehavior:behavior];
}

} // namespace gui
} // namespace digital_clock
