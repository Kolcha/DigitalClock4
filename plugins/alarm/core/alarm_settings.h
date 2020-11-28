/*
    Digital Clock: alarm plugin
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

#ifndef ALARM_PLUGIN_ALARM_SETTINGS_H
#define ALARM_PLUGIN_ALARM_SETTINGS_H

#include <QSettings>

namespace alarm_plugin {

const char* const OPT_STOP_ALARM_SHORTCUT = "stop_shortcut";
const char* const OPT_SETTINGS_SHORTCUT   = "settings_shortcut";
const char* const OPT_LAST_MEDIA_PATH     = "last_media_path";

void InitDefaults(QSettings::SettingsMap* defaults);

} // namespace alarm_plugin

#endif // ALARM_PLUGIN_ALARM_SETTINGS_H
