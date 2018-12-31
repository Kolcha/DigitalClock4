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

#include "core/settings.h"

#include <QDateTime>

namespace countdown_timer {

void InitDefaults(QSettings::SettingsMap* defaults)
{
  defaults->insert(OPT_INTERVAL_HOURS, 0);
  defaults->insert(OPT_INTERVAL_MINUTES, 0);
  defaults->insert(OPT_INTERVAL_SECONDS, 0);

  defaults->insert(OPT_USE_TARGET_TIME, true);
  defaults->insert(OPT_TARGET_DATETIME, QDateTime(QDate(QDate::currentDate().year() + 1, 1, 1)));

  defaults->insert(OPT_SHOW_MESSAGE, false);
  defaults->insert(OPT_MESSAGE_TEXT, QString());

  defaults->insert(OPT_WIDGET_IS_VISIBLE, true);
}

} // namespace countdown_timer
