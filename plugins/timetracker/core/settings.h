/*
    Digital Clock: timetracker plugin
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
    along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef TIMETRACKER_SETTINGS_H
#define TIMETRACKER_SETTINGS_H

#include <QSettings>

namespace timetracker {

const char* const OPT_PAUSE_HOTKEY      = "pause_hotkey";
const char* const OPT_RESTART_HOTKEY    = "restart_hotkey";
const char* const OPT_SETTINGS_HOTKEY   = "settings_hotkey";

const char* const OPT_HIDE_INACTIVE     = "hide_inactive";

void InitDefaults(QSettings::SettingsMap* defaults);

} // namespace timetracker

#endif // TIMETRACKER_SETTINGS_H
