/*
    Digital Clock: schedule plugin
    Copyright (C) 2016-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "tasks_storage.h"

#include "settings_storage.h"

namespace schedule {

TasksStorage::TasksStorage(SettingsStorage* backend, QObject* parent) : SettingsStorageWrapper(backend, parent)
{
}

void TasksStorage::loadDates()
{
  emit datesLoaded(listDates());
}

void TasksStorage::LoadTasks(const QDate& dt)
{
  QList<TaskPtr> tasks;
  QString date_key = QString("plugins/schedule/tasks/%1").arg(dt.toString("dd-MM-yyyy"));
  QStringList items = this->GetBackend()->ListChildren(date_key);
  for (auto& i : qAsConst(items)) {
    bool id_ok = false;
    int id = i.toInt(&id_ok);
    if (!id_ok) continue;
    QString task_key = QString("%1/%2").arg(date_key).arg(id);
    TaskPtr task(new Task());
    task->setId(id);
    task->setDate(dt);
    task->setTime(this->getValue(QString("%1/%2").arg(task_key, "time")).toTime());
    task->setNote(this->getValue(QString("%1/%2").arg(task_key, "note")).toString());
    Notification nt;
    nt.setType(this->getValue(QString("%1/%2").arg(task_key, "notification/type")).value<Notification::Type>());
    nt.setTimeout(this->getValue(QString("%1/%2").arg(task_key, "notification/timeout")).toInt());
    task->setNotification(nt);
    if (task->isValid()) tasks.append(task);
  }
  emit tasksLoaded(tasks);
}

void TasksStorage::addTask(const TaskPtr& task)
{
  if (task->note().isEmpty()) return;

  QString date_key = QString("plugins/schedule/tasks/%1").arg(task->date().toString("dd-MM-yyyy"));
  QStringList items = this->GetBackend()->ListChildren(date_key);
  std::sort(items.begin(), items.end());
  task->setId(items.isEmpty() ? 1 : items.back().toInt() + 1);

  int old_count = listDates().size();

  QString task_key = QString("%1/%2").arg(date_key).arg(task->id());
  this->setValue(QString("%1/%2").arg(task_key, "time"), task->time());
  this->setValue(QString("%1/%2").arg(task_key, "note"), task->note());
  this->setValue(QString("%1/%2").arg(task_key, "notification/type"), task->notification().type());
  this->setValue(QString("%1/%2").arg(task_key, "notification/timeout"), task->notification().timeout());

  QList<QDate> new_dates = listDates();
  if (new_dates.size() != old_count) emit datesLoaded(new_dates);
}

void TasksStorage::delTask(const TaskPtr& task)
{
  QString date_key = QString("plugins/schedule/tasks/%1").arg(task->date().toString("dd-MM-yyyy"));
  QString task_key = QString("%1/%2").arg(date_key).arg(task->id());
  this->remove(task_key);
  if (GetBackend()->ListChildren(date_key).isEmpty()) {
    this->remove(date_key);
  }
}

void TasksStorage::updateTask(const TaskPtr& task)
{
  if (task->note().isEmpty()) return;

  QString date_key = QString("plugins/schedule/tasks/%1").arg(task->date().toString("dd-MM-yyyy"));
  QString task_key = QString("%1/%2").arg(date_key).arg(task->id());

  this->setValue(QString("%1/%2").arg(task_key, "time"), task->time());
  this->setValue(QString("%1/%2").arg(task_key, "note"), task->note());
  this->setValue(QString("%1/%2").arg(task_key, "notification/type"), task->notification().type());
  this->setValue(QString("%1/%2").arg(task_key, "notification/timeout"), task->notification().timeout());
}

QList<QDate> TasksStorage::listDates() const
{
  QList<QDate> dates;
  QStringList items = this->GetBackend()->ListChildren("plugins/schedule/tasks");
  for (auto& i : qAsConst(items)) {
    QDate dt = QDate::fromString(i, "dd-MM-yyyy");
    if (!dt.isValid()) continue;
    dates.append(dt);
  }
  return dates;
}

} // namespace schedule
