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

const char* const OPT_EVERY_HOUR_ENABLED   = "every_hour/enabled";
const char* const OPT_EVERY_HOUR_FORMAT    = "every_hour/format";
const char* const OPT_QUARTER_HOUR_ENABLED = "quarter_hour/enabled";
const char* const OPT_QUARTER_HOUR_FORMAT  = "quarter_hour/format";

const char* const OPT_SYNTHESIS_VOLUME     = "volume";
const char* const OPT_SYNTHESIS_RATE       = "synthesis/rate";
const char* const OPT_SYNTHESIS_PITCH      = "synthesis/pitch";
const char* const OPT_SYNTHESIS_ENGINE     = "synthesis/engine";
const char* const OPT_SYNTHESIS_LANGUAGE   = "synthesis/language";
const char* const OPT_SYNTHESIS_VOICE      = "synthesis/voice";

void InitDefaults(QSettings::SettingsMap* defaults);

} // namespace talking_clock

#endif // TALKING_CLOCK_SETTINGS_H
