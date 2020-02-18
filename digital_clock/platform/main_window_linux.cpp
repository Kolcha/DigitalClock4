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

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <QX11Info>

namespace digital_clock {
namespace gui {

static unsigned long GetCurrentDesktop(Display* display)
{
  Atom actual_type_return;
  int actual_format_return = 0;
  unsigned long nitems_return = 0;
  unsigned long bytes_after_return = 0;
  unsigned long* desktop = nullptr;
  unsigned long ret;

  if (XGetWindowProperty(display, DefaultRootWindow(display),
                         XInternAtom(display, "_NET_CURRENT_DESKTOP", False),
                         0, 1, False, XA_CARDINAL,
                         &actual_type_return, &actual_format_return,
                         &nitems_return, &bytes_after_return,
                         reinterpret_cast<unsigned char**>(&desktop)) != Success) {
    return 0;
  }
  if (actual_type_return != XA_CARDINAL || nitems_return == 0) {
    return 0;
  }

  ret = desktop[0];
  XFree(desktop);
  return ret;
}

void ClockWindow::SetVisibleOnAllDesktops(bool set)
{
  if (!QX11Info::isPlatformX11()) return;

  unsigned long desktop = GetCurrentDesktop(QX11Info::display());
  // http://stackoverflow.com/questions/16775352/keep-a-application-window-always-on-current-desktop-on-linux-and-mac/
  unsigned long data = set ? 0xFFFFFFFF : desktop;
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
