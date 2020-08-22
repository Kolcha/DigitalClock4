/*
    Digital Clock: countdown timer plugin
    Copyright (C) 2017-2020  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "core/settings.h"

#include <QDateTime>

namespace countdown_timer {

QDateTime GetDefaultDate()
{
  return QDateTime(QDate(QDate::currentDate().year() + 1, 1, 1));
}

void InitDefaults(QSettings::SettingsMap* defaults)
{
  defaults->insert(OPT_INTERVAL_HOURS, 0);
  defaults->insert(OPT_INTERVAL_MINUTES, 0);
  defaults->insert(OPT_INTERVAL_SECONDS, 0);

  defaults->insert(OPT_USE_TARGET_TIME, true);
  defaults->insert(OPT_TARGET_DATETIME, GetDefaultDate());

  defaults->insert(OPT_CHIME_ON_TIMEOUT, false);
  defaults->insert(OPT_CHIME_SOUND_FILE, QString());

  defaults->insert(OPT_SHOW_MESSAGE, false);
  defaults->insert(OPT_MESSAGE_TEXT, QString());

  defaults->insert(OPT_HIDE_DAYS_THRESHOLD, 0);

  defaults->insert(OPT_RESTART_ON_DBLCLIK, false);
  defaults->insert(OPT_RESTART_ON_TIMEOUT, false);

  defaults->insert(OPT_PAUSE_HOTKEY, QString());
  defaults->insert(OPT_RESTART_HOTKEY, QString());
}

} // namespace countdown_timer
