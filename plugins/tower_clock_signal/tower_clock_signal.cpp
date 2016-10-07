/*
    Digital Clock: tower clock signal plugin
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

#include "tower_clock_signal.h"

#include <QSound>
#include <QTime>


namespace tower_clock_signal {

TowerCLockSignal::TowerCLockSignal()
{
  InitTranslator(QLatin1String(":/tower_clock_signal/tower_clock_signal_"));
  info_.display_name = tr("Tower clock hour signal (bell per hour)");
  info_.description = tr("Plays bell strikes every hour like tower clock.");
  InitIcon(":/tower_clock_signal/icon.svg");
}

void TowerCLockSignal::Start()
{
  bell_sound_ = new QSound(":/tower_clock_signal/bell.wav", this);
}

void TowerCLockSignal::Stop()
{
  bell_sound_->stop();
}

void TowerCLockSignal::TimeUpdateListener()
{
  QTime curr_time = QTime::currentTime();
  if (curr_time.minute() != 0 || curr_time.second() != 0 ||
      !bell_sound_->isFinished() || bell_sound_->loopsRemaining() != 0) return;

  int hour = curr_time.hour();
  if (hour > 12) hour -= 12;
  if (hour == 0) hour  = 12;

  Q_ASSERT(1 <= hour && hour <= 12);

  bell_sound_->setLoops(hour);
  bell_sound_->play();
}

} // namespace tower_clock_signal
