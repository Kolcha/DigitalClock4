/*
    Digital Clock: custom signal plugin
    Copyright (C) 2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef CUSTOM_SIGNAL_SETTINGS_H
#define CUSTOM_SIGNAL_SETTINGS_H

#include <QSettings>

namespace custom_signal {

#define OPT_EVERY_HOUR_ENABLED        "plugins/custom_signal/every_hour/enabled"
#define OPT_EVERY_HOUR_SIGNAL         "plugins/custom_signal/every_hour/signal"
#define OPT_EVERY_HOUR_VOLUME         "plugins/custom_signal/every_hour/volume"

#define OPT_QUARTER_HOUR_ENABLED      "plugins/custom_signal/quarter_hour/enabled"
#define OPT_QUARTER_HOUR_SIGNAL       "plugins/custom_signal/quarter_hour/signal"
#define OPT_QUARTER_HOUR_VOLUME       "plugins/custom_signal/quarter_hour/volume"

#define OPT_CUSTOM_PERIOD_ENABLED     "plugins/custom_signal/custom/enabled"
#define OPT_CUSTOM_PERIOD_SIGNAL      "plugins/custom_signal/custom/signal"
#define OPT_CUSTOM_PERIOD_VOLUME      "plugins/custom_signal/custom/volume"
#define OPT_CUSTOM_PERIOD_PERIOD      "plugins/custom_signal/custom/period"
#define OPT_CUSTOM_PERIOD_OFFSET      "plugins/custom_signal/custom/offset"

#define OPT_EVERY_HOUR_LAST_FILE      "plugins/custom_signal/state/last_every_hour_file"
#define OPT_QUARTER_HOUR_LAST_FILE    "plugins/custom_signal/state/last_quarter_hour_file"
#define OPT_CUSTOM_PERIOD_LAST_FILE   "plugins/custom_signal/state/last_custom_period_file"

void InitDefaults(QSettings::SettingsMap* defaults);

} // namespace custom_signal

#endif // CUSTOM_SIGNAL_SETTINGS_H
