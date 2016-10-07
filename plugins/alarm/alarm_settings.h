/*
    Digital Clock: alarm plugin
    Copyright (C) 2013-2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef ALARM_SETTINGS_H
#define ALARM_SETTINGS_H

#include <QSettings>
#include <QTime>

namespace alarm {

// alarm plugin settings keys
#define OPT_ENABLED           "plugins/alarm/enabled"
#define OPT_TIME              "plugins/alarm/time"
#define OPT_SIGNAL_TYPE       "plugins/alarm/signal_type"
#define OPT_FILENAME          "plugins/alarm/filename"
#define OPT_STREAM_URL        "plugins/alarm/stream_url"
#define OPT_VOLUME            "plugins/alarm/volume"
#define OPT_SHOW_NOTIFY       "plugins/alarm/show_notify"
#define OPT_NOTIFY_TEXT       "plugins/alarm/notify_text"

enum SignalType { ST_FILE, ST_STREAM };

inline void InitDefaults(QSettings::SettingsMap* defaults)
{
  defaults->insert(OPT_ENABLED, false);
  defaults->insert(OPT_TIME, QTime());
  defaults->insert(OPT_SIGNAL_TYPE, (int)ST_FILE);
  defaults->insert(OPT_FILENAME, QString());
  defaults->insert(OPT_STREAM_URL, QString());
  defaults->insert(OPT_VOLUME, 50);
  defaults->insert(OPT_SHOW_NOTIFY, false);
  defaults->insert(OPT_NOTIFY_TEXT, QString());
}

} // namespace alarm

#endif // ALARM_SETTINGS_H
