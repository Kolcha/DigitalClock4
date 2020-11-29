/*
    Digital Clock: schedule plugin
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

#ifndef SCHEDULE_PLUGIN_SCHEDULE_SETTINGS_H
#define SCHEDULE_PLUGIN_SCHEDULE_SETTINGS_H

#include <QSettings>

namespace schedule {

const char* const OPT_NOTIFICATION_TYPE   = "defaults/notification_type";
const char* const OPT_NOTIFICATION_TIME   = "defaults/notofocation_time";
const char* const OPT_PLAY_SOUND          = "defaults/play_sound";
const char* const OPT_SOUND_FILE          = "defaults/sound_file";
const char* const OPT_SHOW_TRAY_ICON      = "settings/show_tray_icon";
const char* const OPT_ADD_TASK_SHORTCUT_E = "settings/add_task_shortcut_enabled";
const char* const OPT_ADD_TASK_SHORTCUT   = "settings/add_task_shortcut";

void InitDefaults(QSettings::SettingsMap* defaults);

} // namespace schedule

#endif // SCHEDULE_PLUGIN_SCHEDULE_SETTINGS_H
