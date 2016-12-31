/*
    Digital Clock: talking clock plugin
    Copyright (C) 2016-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef TALKING_CLOCK_SETTINGS_H
#define TALKING_CLOCK_SETTINGS_H

#include <QSettings>

namespace talking_clock {

#define OPT_EVERY_HOUR_ENABLED        "plugins/talking_clock/every_hour/enabled"
#define OPT_EVERY_HOUR_FORMAT         "plugins/talking_clock/every_hour/format"
#define OPT_QUARTER_HOUR_ENABLED      "plugins/talking_clock/quarter_hour/enabled"
#define OPT_QUARTER_HOUR_FORMAT       "plugins/talking_clock/quarter_hour/format"

#define OPT_SYNTHESIS_VOLUME          "plugins/talking_clock/volume"
#define OPT_SYNTHESIS_RATE            "plugins/talking_clock/synthesis/rate"
#define OPT_SYNTHESIS_PITCH           "plugins/talking_clock/synthesis/pitch"
#define OPT_SYNTHESIS_ENGINE          "plugins/talking_clock/synthesis/engine"
#define OPT_SYNTHESIS_LANGUAGE        "plugins/talking_clock/synthesis/language"
#define OPT_SYNTHESIS_VOICE           "plugins/talking_clock/synthesis/voice"

void InitDefaults(QSettings::SettingsMap* defaults);

} // namespace talking_clock

#endif // TALKING_CLOCK_SETTINGS_H
