/*
    Digital Clock: hour signal plugin
    Copyright (C) 2013-2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#include <QTime>
#include <QSound>
#include "hour_signal.h"

namespace hour_signal {

HourSignal::HourSignal() : last_hour_(-1)
{
  InitTranslator(QLatin1String(":/hour_signal/hour_signal_"));
  info_.display_name = tr("Every hour signal");
  info_.description = tr("Plays short signal every hour.");
  InitIcon(":/hour_signal/icon.svg");
}

void HourSignal::TimeUpdateListener()
{
  int current_hour = QTime::currentTime().toString("H").toInt();
  if (current_hour != last_hour_ && last_hour_ != -1) QSound::play(":/hour_signal/hour_signal.wav");
  last_hour_ = current_hour;
}

} // namespace hour_signal
