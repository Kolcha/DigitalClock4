/*
    Digital Clock: timetracker plugin
    Copyright (C) 2018-2020  Nick Korotysh <nick.korotysh@gmail.com>

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
#include "timetracker.h"

namespace timetracker {

Timetracker::Timetracker(QObject* parent) : QObject(parent)
{
  timer_.setInterval(1000);
  connect(&timer_, &QTimer::timeout, [this] { ++counter_; });
}

int Timetracker::elapsed() const
{
  return counter_ * timer_.interval() / 1000;
}

bool Timetracker::isActive() const
{
  return timer_.isActive();
}

void Timetracker::start()
{
  timer_.start();
}

void Timetracker::stop()
{
  timer_.stop();
}

void Timetracker::reset()
{
  bool was_active = timer_.isActive();
  if (was_active) stop();
  counter_ = 0;
  if (was_active) start();
}

} // namespace timetracker
