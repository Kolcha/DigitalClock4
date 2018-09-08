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

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

#include "core/clock_settings.h"
#include "platform/fullscreen_detect.h"

namespace digital_clock {
namespace gui {

void ClockWindow::WinOnTopWorkaround()
{
  // always on top problem workaround
  // https://sourceforge.net/p/digitalclock4/tickets/3/
  // https://sourceforge.net/p/digitalclock4/tickets/9/
  if (app_config_->GetValue(OPT_STAY_ON_TOP).toBool()) {
    if (fullscreen_detect_enabled_ && IsFullscreenWndOnSameMonitor(this->winId(), window_ignore_list_)) {
      if (this->windowFlags() & Qt::WindowStaysOnTopHint) {
        this->SetWindowFlag(Qt::WindowStaysOnTopHint, false);
        this->lower();
      }
      return;
    } else {
      if (!(this->windowFlags() & Qt::WindowStaysOnTopHint)) {
        this->SetWindowFlag(Qt::WindowStaysOnTopHint, true);
      }
      // https://forum.qt.io/topic/28739/flags-windows-7-window-always-on-top-including-the-win7-taskbar-custom-error/4
      if (!this->isActiveWindow()) this->raise();
    }
  }
}

void ClockWindow::KeepOnDesktop()
{
  // keep window visible after Win+D (show desktop)
  // https://sourceforge.net/p/digitalclock4/tickets/28/
  // https://stackoverflow.com/questions/35045060/how-to-keep-window-visible-at-all-times-but-not-force-it-to-be-on-top
  HWND hWndTmp = FindWindowEx(NULL, NULL, L"Progman", NULL);
  if (hWndTmp) {
    hWndTmp = FindWindowEx(hWndTmp, NULL, L"SHELLDLL_DefView", NULL);
    if (hWndTmp) {
      SetWindowLongPtr((HWND)this->winId(), GWLP_HWNDPARENT, (LONG_PTR)hWndTmp);
    }
  }
}

} // namespace gui
} // namespace digital_clock
