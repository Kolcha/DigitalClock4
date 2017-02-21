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

#ifndef SCHEDULE_TASKS_STORAGE_H
#define SCHEDULE_TASKS_STORAGE_H

#include "settings_storage_wrapper.h"

#include <QList>

#include "core/task.h"

namespace schedule {

class TasksStorage : public SettingsStorageWrapper
{
  Q_OBJECT
public:
  explicit TasksStorage(SettingsStorage* backend, QObject* parent = 0);

signals:
  void datesLoaded(const QList<QDate>& dates);
  void tasksLoaded(const QList<TaskPtr>& lst);

public slots:
  void loadDates();
  void LoadTasks(const QDate& dt);

  void addTask(const TaskPtr& task);
  void delTask(const TaskPtr& task);

private:
  QList<QDate> listDates() const;
};

} // namespace schedule

#endif // SCHEDULE_TASKS_STORAGE_H
