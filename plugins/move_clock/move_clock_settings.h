/*
    Digital Clock: move clock plugin
    Copyright (C) 2014-2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef MOVE_CLOCK_SETTINGS_H
#define MOVE_CLOCK_SETTINGS_H

#include <QSettings>

namespace move_clock {
// move_clock plugin settings
#define OPT_X0        "plugins/move_clock/x0"
#define OPT_KX        "plugins/move_clock/kx"
#define OPT_AX        "plugins/move_clock/ax"
#define OPT_NX        "plugins/move_clock/nx"
#define OPT_Y0        "plugins/move_clock/y0"
#define OPT_KY        "plugins/move_clock/ky"
#define OPT_AY        "plugins/move_clock/ay"
#define OPT_NY        "plugins/move_clock/ny"
#define OPT_TIMEOUT   "plugins/move_clock/timeout"


inline void InitDefaults(QSettings::SettingsMap* defaults)
{
  defaults->insert(OPT_X0, 50);
  defaults->insert(OPT_KX, -2.0);
  defaults->insert(OPT_AX, 0);
  defaults->insert(OPT_NX, 0.01);
  defaults->insert(OPT_Y0, 20);
  defaults->insert(OPT_KY, 0.0);
  defaults->insert(OPT_AY, 0);
  defaults->insert(OPT_NY, 0.01);
  defaults->insert(OPT_TIMEOUT, 200);
}

} // namespace move_clock

#endif // MOVE_CLOCK_SETTINGS_H
