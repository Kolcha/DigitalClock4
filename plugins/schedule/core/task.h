/*
    Digital Clock: schedule plugin
    Copyright (C) 2013-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef SCHEDULE_TASK_H
#define SCHEDULE_TASK_H

#include <QString>
#include <QDateTime>
#include <QSharedPointer>

namespace schedule {

class Task
{
public:
  Task();

  int id() const;

  QDate date() const;
  QTime time() const;

  QString note() const;

  void setId(const int id);

  void setDate(const QDate& dt);
  void setTime(const QTime& tm);

  void setNote(const QString& msg);

  bool isValid() const;

private:
  int id_;
  QDateTime date_time_;
  QString message_;
};

typedef QSharedPointer<Task> TaskPtr;

} // namespace schedule

#endif // SCHEDULE_TASK_H
