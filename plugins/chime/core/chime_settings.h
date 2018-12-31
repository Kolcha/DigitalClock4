/*
    Digital Clock: chime plugin
    Copyright (C) 2016-2019  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef CHIME_CHIME_SETTINGS_H
#define CHIME_CHIME_SETTINGS_H

#include <QSettings>

namespace chime {

const char* const OPT_EVERY_HOUR_ENABLED     = "every_hour/enabled";
const char* const OPT_EVERY_HOUR_SIGNAL      = "every_hour/signal";
const char* const OPT_EVERY_HOUR_VOLUME      = "every_hour/volume";
const char* const OPT_EVERY_HOUR_REPEAT      = "every_hour/repeat";

const char* const OPT_QUARTER_HOUR_ENABLED   = "quarter_hour/enabled";
const char* const OPT_QUARTER_HOUR_SIGNAL    = "quarter_hour/signal";
const char* const OPT_QUARTER_HOUR_VOLUME    = "quarter_hour/volume";
const char* const OPT_QUARTER_HOUR_REPEAT    = "quarter_hour/repeat";

const char* const OPT_QUIET_HOURS_ENABLED    = "quiet_hours/enabled";
const char* const OPT_QUIET_HOURS_START      = "quiet_hours/start_time";
const char* const OPT_QUIET_HOURS_END        = "quiet_hours/end_end";

const char* const OPT_EVERY_HOUR_LAST_FILE   = "state/last_every_hour_file";
const char* const OPT_QUARTER_HOUR_LAST_FILE = "state/last_quarter_hour_file";

void InitDefaults(QSettings::SettingsMap* defaults);

} // namespace chime

#endif // CHIME_CHIME_SETTINGS_H
