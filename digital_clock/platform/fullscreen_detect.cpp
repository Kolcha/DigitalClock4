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

#include "fullscreen_detect.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

namespace digital_clock {

struct wnd_find_t {
  HMONITOR monitor;
  int width;
  int height;
  bool found;
};

// parts of solution was found there:
// http://stackoverflow.com/questions/3797802/how-to-check-if-an-other-program-is-running-in-fullscreen-mode-eg-a-media-play
// http://stackoverflow.com/questions/7009080/detecting-full-screen-mode-in-windows

BOOL CALLBACK CheckMaximized(HWND hwnd, LPARAM lParam)
{
  wnd_find_t* data = (wnd_find_t*)lParam;
  // skip windows from other monitors
  HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
  if (hMonitor != data->monitor) return TRUE;

  // skip desktop window
  if (hwnd == GetDesktopWindow() || hwnd == GetShellWindow()) return TRUE;

  RECT r;
  GetWindowRect(hwnd, &r);

  if((r.right - r.left == data->width) && (r.bottom - r.top == data->height)) {
    data->found = true;
    return FALSE; //there can be only one so quit here
  }

  return TRUE;
}

bool IsFullscreenWndOnSameMonitor(WId wid)
{
  wnd_find_t data;
  data.monitor = MonitorFromWindow((HWND)wid, MONITOR_DEFAULTTONEAREST);
  MONITORINFO info;
  info.cbSize = sizeof(MONITORINFO);
  if (GetMonitorInfo(data.monitor, &info)) {
    data.width = info.rcMonitor.right - info.rcMonitor.left;
    data.height = info.rcMonitor.bottom - info.rcMonitor.top;
  }
  data.found = false;
  EnumWindows(CheckMaximized, (LPARAM)(&data));
  return data.found;
}

} // namespace digital_clock
