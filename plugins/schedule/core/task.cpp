/*
    Digital Clock: schedule plugin
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

#include "task.h"

namespace schedule {

Task::Task() : id_(0)
{
}

int Task::id() const
{
  return id_;
}

QDate Task::date() const
{
  return date_time_.date();
}

QTime Task::time() const
{
  return date_time_.time();
}

QString Task::note() const
{
  return message_;
}

void Task::setId(const int id)
{
  Q_ASSERT(id_ <= 0);
  id_ = id;
}

void Task::setDate(const QDate& dt)
{
  date_time_.setDate(dt);
}

void Task::setTime(const QTime& tm)
{
  date_time_.setTime(tm);
}

void Task::setNote(const QString& msg)
{
  message_ = msg;
}

bool Task::isValid() const
{
  return id() > 0 && date().isValid() && time().isValid() && !note().isEmpty();
}

} // namespace schedule
