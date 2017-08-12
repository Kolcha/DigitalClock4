/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "main_window.h"

#include <objc/objc-runtime.h>

namespace digital_clock {

void MainWindow::SetVisibleOnAllDesktops(bool set)
{
  // http://stackoverflow.com/questions/16775352/keep-a-application-window-always-on-current-desktop-on-linux-and-mac/
  WId windowObject = this->winId();
  objc_object* nsviewObject = reinterpret_cast<objc_object*>(windowObject);
  objc_object* nsWindowObject = objc_msgSend(nsviewObject, sel_registerName("window"));
  int NSWindowCollectionBehaviorCanJoinAllSpaces = set ? 1 << 0 : 0 << 0;
  int NSWindowCollectionBehaviorFullScreenNone = 1 << 9;
  int collectionBehavior = NSWindowCollectionBehaviorCanJoinAllSpaces | NSWindowCollectionBehaviorFullScreenNone;
  objc_msgSend(nsWindowObject, sel_registerName("setCollectionBehavior:"), collectionBehavior);
}

} // namespace digital_clock
