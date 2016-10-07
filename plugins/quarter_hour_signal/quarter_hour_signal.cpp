/*
    Digital Clock: quarter hour signal plugin
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

#include "quarter_hour_signal.h"

#include <QTime>
#include <QSound>

namespace quarter_hour_signal {

QuarterHourSignal::QuarterHourSignal() : last_minute_(-1)
{
  InitTranslator(QLatin1String(":/quarter_hour_signal/quarter_hour_signal_"));
  info_.display_name = tr("Quarter of an hour signal");
  info_.description = tr("Chiming on every quarter of an hour.");
  InitIcon(":/quarter_hour_signal/icon.svg");
}

void QuarterHourSignal::TimeUpdateListener()
{
  int cur_min = QTime::currentTime().minute();
  if (cur_min == 0 || cur_min == 15 || cur_min == 30 || cur_min == 45) {
    if (cur_min != last_minute_) {
      QSound::play(":/quarter_hour_signal/signal.wav");
    }
  }
  last_minute_ = cur_min;
}

} // namespace quarter_hour_signal
