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

#include "countdown_timer.h"

namespace countdown_timer {

CountdownTimer::CountdownTimer(QObject* parent) :
  QObject(parent),
  timer_id_(-1), interval_(-1), time_left_(-1),
  is_active_(false)
{
  connect(this, &CountdownTimer::timeout, this, &CountdownTimer::stop);
}

CountdownTimer::~CountdownTimer()
{
  if (isActive()) stop();
}

void CountdownTimer::start()
{
  if (isActive() || timeLeft() <= 0) return;
  timer_id_ = startTimer(1000);
  is_active_ = true;
  emit activityChanged(is_active_);
}

void CountdownTimer::stop()
{
  if (!isActive()) return;
  killTimer(timer_id_);
  is_active_ = false;
  emit activityChanged(is_active_);
}

void CountdownTimer::setInterval(int seconds)
{
  setTimeLeft(seconds);
  interval_ = seconds;
}

void CountdownTimer::timerEvent(QTimerEvent* event)
{
  if (timeLeft() > 0) setTimeLeft(timeLeft() - 1);
  if (timeLeft() == 0) emit timeout();
  QObject::timerEvent(event);
}

void CountdownTimer::setTimeLeft(qint64 time_left)
{
  if (time_left < 0 || time_left == time_left_) return;
  time_left_ = time_left;
  emit timeLeftChanged(time_left_);
}

} // namespace countdown_timer
