/*
    Digital Clock: countdown timer plugin
    Copyright (C) 2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#define OPT_INTERVAL_HOURS            "plugins/countdown_timer/hours"
#define OPT_INTERVAL_MINUTES          "plugins/countdown_timer/minutes"
#define OPT_INTERVAL_SECONDS          "plugins/countdown_timer/seconds"

#define OPT_USE_TARGET_TIME           "plugins/countdown_timer/use_target_time"
#define OPT_TARGET_DATETIME           "plugins/countdown_timer/target_datetime"

#define OPT_SHOW_MESSAGE              "plugins/countdown_timer/show_message"
#define OPT_MESSAGE_TEXT              "plugins/countdown_timer/message_text"

#define OPT_WIDGET_IS_VISIBLE         "plugins/countdown_timer/show_widget"

void InitDefaults(QSettings::SettingsMap* defaults);

} // namespace countdown_timer

#endif // COUNTDOWN_TIMER_SETTINGS_H
