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

#include "talking_clock_settings.h"

namespace talking_clock {

void InitDefaults(QSettings::SettingsMap* defaults)
{
  defaults->insert(OPT_EVERY_HOUR_ENABLED, true);
  defaults->insert(OPT_EVERY_HOUR_FORMAT, QString("'It''s' ha 'o''clock'"));
  defaults->insert(OPT_QUARTER_HOUR_ENABLED, false);
  defaults->insert(OPT_QUARTER_HOUR_FORMAT, QString("h:m ap"));

  defaults->insert(OPT_SYNTHESIS_VOLUME, 70);
  defaults->insert(OPT_SYNTHESIS_RATE, 0);
  defaults->insert(OPT_SYNTHESIS_PITCH, 0);
  defaults->insert(OPT_SYNTHESIS_ENGINE, QString("default"));
  defaults->insert(OPT_SYNTHESIS_LANGUAGE, -1);
  defaults->insert(OPT_SYNTHESIS_VOICE, -1);
}

} // namespace talking_clock
