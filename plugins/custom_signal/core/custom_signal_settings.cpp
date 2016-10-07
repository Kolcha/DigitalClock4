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

#include "custom_signal_settings.h"

#include <QUrl>

namespace custom_signal {

void InitDefaults(QSettings::SettingsMap* defaults)
{
  defaults->insert(OPT_EVERY_HOUR_ENABLED, true);
  defaults->insert(OPT_EVERY_HOUR_SIGNAL, QUrl("qrc:/custom_signal/eh_signal.wav"));
  defaults->insert(OPT_EVERY_HOUR_VOLUME, 90);
  defaults->insert(OPT_EVERY_HOUR_LAST_FILE, QString());

  defaults->insert(OPT_QUARTER_HOUR_ENABLED, false);
  defaults->insert(OPT_QUARTER_HOUR_SIGNAL, QUrl("qrc:/custom_signal/qh_signal.wav"));
  defaults->insert(OPT_QUARTER_HOUR_VOLUME, 60);
  defaults->insert(OPT_QUARTER_HOUR_LAST_FILE, QString());

  defaults->insert(OPT_CUSTOM_PERIOD_ENABLED, false);
  defaults->insert(OPT_CUSTOM_PERIOD_SIGNAL, QUrl("qrc:/custom_signal/bell.wav"));
  defaults->insert(OPT_CUSTOM_PERIOD_VOLUME, 30);
  defaults->insert(OPT_CUSTOM_PERIOD_PERIOD, 10);
  defaults->insert(OPT_CUSTOM_PERIOD_OFFSET, 0);
  defaults->insert(OPT_CUSTOM_PERIOD_LAST_FILE, QString());
}

} // namespace custom_signal
