/*
    Digital Clock: chime plugin
    Copyright (C) 2016-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "chime_settings.h"

#include <QUrl>
#include <QTime>

#include "core/repeat_type.h"


namespace chime {

void InitDefaults(QSettings::SettingsMap* defaults)
{
  defaults->insert(OPT_EVERY_HOUR_ENABLED, true);
  defaults->insert(OPT_EVERY_HOUR_SIGNAL, QUrl("qrc:/chime/hour_signal.wav"));
  defaults->insert(OPT_EVERY_HOUR_VOLUME, 90);
  defaults->insert(OPT_EVERY_HOUR_REPEAT, static_cast<int>(Repeat::Once));
  defaults->insert(OPT_EVERY_HOUR_LAST_FILE, QString());

  defaults->insert(OPT_QUARTER_HOUR_ENABLED, false);
  defaults->insert(OPT_QUARTER_HOUR_SIGNAL, QUrl("qrc:/chime/quarter_signal.wav"));
  defaults->insert(OPT_QUARTER_HOUR_VOLUME, 90);
  defaults->insert(OPT_QUARTER_HOUR_REPEAT, static_cast<int>(Repeat::Once));
  defaults->insert(OPT_QUARTER_HOUR_LAST_FILE, QString());

  defaults->insert(OPT_QUIET_HOURS_ENABLED, false);
  defaults->insert(OPT_QUIET_HOURS_START, QTime(23, 1));
  defaults->insert(OPT_QUIET_HOURS_END, QTime(6, 59));
}

} // namespace chime
