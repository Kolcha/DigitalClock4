/*
    Digital Clock: schedule plugin
    Copyright (C) 2016-2020  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef SCHEDULE_TASKS_INVOKER_H
#define SCHEDULE_TASKS_INVOKER_H

#include <QObject>

#include <QDateTime>
#include <QList>
#include <QMap>

#include "core/task.h"

class QTimer;

namespace schedule {

class TasksInvoker : public QObject
{
  Q_OBJECT
public:
  explicit TasksInvoker(QObject* parent = nullptr);

signals:
  void dateChanged(const QDate& date);
  void completed(const TaskPtr& task);
  void done();

public slots:
  void setDailyTasks(const QList<TaskPtr>& tasks);

  void start();
  void stop();

private slots:
  void timeoutHandler();

private:
  QTimer* timer_;
  QDate today_;
  QMultiMap<QTime, TaskPtr> tasks_;
};

} // namespace schedule

#endif // SCHEDULE_TASKS_INVOKER_H
