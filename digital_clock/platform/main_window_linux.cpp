/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2017-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <QX11Info>

namespace digital_clock {
namespace gui {

void ClockWindow::SetVisibleOnAllDesktops(bool set)
{
  // http://stackoverflow.com/questions/16775352/keep-a-application-window-always-on-current-desktop-on-linux-and-mac/
  unsigned int data = set ? 0xFFFFFFFF : 0x00000000;
  XChangeProperty(QX11Info::display(),
                  winId(),
                  XInternAtom(QX11Info::display(), "_NET_WM_DESKTOP", False),
                  XA_CARDINAL,
                  32,
                  PropModeReplace,
                  reinterpret_cast<unsigned char*>(&data),  // all desktop
                  1);
}

} // namespace gui
} // namespace digital_clock
