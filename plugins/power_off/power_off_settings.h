/*
    Digital Clock: power off plugin
    Copyright (C) 2013-2020  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef POWER_OFF_SETTINGS_H
#define POWER_OFF_SETTINGS_H

#include <QSettings>
#include <QTime>

namespace power_off {

// power_off plugin settings
const char* const OPT_TIME  = "time";
const char* const OPT_FORCE = "force";

inline void InitDefaults(QSettings::SettingsMap* defaults)
{
  defaults->insert(OPT_TIME, QTime());
  defaults->insert(OPT_FORCE, false);
}

} // namespace power_off

#endif // POWER_OFF_SETTINGS_H
