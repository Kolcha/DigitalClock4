/*
    Digital Clock: chime plugin
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

#ifndef CHIME_CHIME_SETTINGS_H
#define CHIME_CHIME_SETTINGS_H

#include <QSettings>

namespace chime {

#define OPT_EVERY_HOUR_ENABLED        "plugins/chime/every_hour/enabled"
#define OPT_EVERY_HOUR_SIGNAL         "plugins/chime/every_hour/signal"
#define OPT_EVERY_HOUR_VOLUME         "plugins/chime/every_hour/volume"
#define OPT_EVERY_HOUR_REPEAT         "plugins/chime/every_hour/repeat"

#define OPT_QUARTER_HOUR_ENABLED      "plugins/chime/quarter_hour/enabled"
#define OPT_QUARTER_HOUR_SIGNAL       "plugins/chime/quarter_hour/signal"
#define OPT_QUARTER_HOUR_VOLUME       "plugins/chime/quarter_hour/volume"
#define OPT_QUARTER_HOUR_REPEAT       "plugins/chime/quarter_hour/repeat"

#define OPT_EVERY_HOUR_LAST_FILE      "plugins/chime/state/last_every_hour_file"
#define OPT_QUARTER_HOUR_LAST_FILE    "plugins/chime/state/last_quarter_hour_file"

void InitDefaults(QSettings::SettingsMap* defaults);

} // namespace chime

#endif // CHIME_CHIME_SETTINGS_H
