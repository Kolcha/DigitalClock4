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

#include "system_theme_tracker.h"

#include <Windows.h>

namespace digital_clock {

class SystemThemeTrackerData
{
public:
  bool is_light_theme = false;
  bool should_stop = false;
  HKEY opened_key = NULL;
  HANDLE reg_event = NULL;
};

SystemThemeTracker::SystemThemeTracker(QObject* parent)
  : QThread(parent)
  , data_(new SystemThemeTrackerData())
{
  RegOpenKeyEx(HKEY_CURRENT_USER,
               TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize"),
               0,
               KEY_QUERY_VALUE | KEY_NOTIFY,
               &data_->opened_key);
  data_->is_light_theme = isLightTheme();
  data_->reg_event = CreateEvent(NULL, FALSE, FALSE, NULL);
}

SystemThemeTracker::~SystemThemeTracker()
{
  RegCloseKey(data_->opened_key);
  CloseHandle(data_->reg_event);
  delete data_;
}

bool SystemThemeTracker::isLightTheme() const
{
  unsigned char buffer[sizeof(DWORD)];
  DWORD size = sizeof(buffer);
  LONG ec = RegQueryValueEx(data_->opened_key,
                            TEXT("SystemUsesLightTheme"),
                            NULL, NULL,
                            (LPBYTE)buffer, &size);
  DWORD value = 0;
  if (ec == ERROR_SUCCESS)
    value = *reinterpret_cast<DWORD*>(buffer);
  return value != 0;
}

void SystemThemeTracker::start()
{
  QThread::start(QThread::LowPriority);
}

void SystemThemeTracker::stop()
{
  data_->should_stop = true;
  SetEvent(data_->reg_event);
}

void SystemThemeTracker::run()
{
  while (!data_->should_stop) {
    if (!data_->reg_event)
      break;

    LONG ec = RegNotifyChangeKeyValue(data_->opened_key,
                                      FALSE,
                                      REG_NOTIFY_CHANGE_LAST_SET,
                                      data_->reg_event, TRUE);
    if (ec != ERROR_SUCCESS)
      break;

    if (WaitForSingleObject(data_->reg_event, INFINITE) != WAIT_OBJECT_0)
      break;

    bool is_light_theme = isLightTheme();
    if (is_light_theme != data_->is_light_theme) {
      data_->is_light_theme = is_light_theme;
      emit themeChanged(is_light_theme);
    }
  }
  data_->should_stop = false;
}

} // namespace digital_clock
