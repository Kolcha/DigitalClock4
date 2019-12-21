/*
    Digital Clock: countdown timer plugin
    Copyright (C) 2017-2019  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef COUNTDOWN_TIMER_SETTINGS_H
#define COUNTDOWN_TIMER_SETTINGS_H

#include <QSettings>

namespace countdown_timer {

const char* const OPT_INTERVAL_HOURS    = "hours";
const char* const OPT_INTERVAL_MINUTES  = "minutes";
const char* const OPT_INTERVAL_SECONDS  = "seconds";

const char* const OPT_USE_TARGET_TIME   = "use_target_time";
const char* const OPT_TARGET_DATETIME   = "target_datetime";

const char* const OPT_SHOW_MESSAGE      = "show_message";
const char* const OPT_MESSAGE_TEXT      = "message_text";

const char* const OPT_HIDE_DAYS_THRESHOLD = "hide_days_threshold";

QDateTime GetDefaultDate();

void InitDefaults(QSettings::SettingsMap* defaults);

} // namespace countdown_timer

#endif // COUNTDOWN_TIMER_SETTINGS_H
