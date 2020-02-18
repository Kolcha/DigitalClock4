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

#include "daily_tasks_provider.h"

namespace schedule {

DailyTasksProvider::DailyTasksProvider(QObject* parent)
  : QAbstractTableModel(parent)
{
}

QVariant DailyTasksProvider::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation != Qt::Horizontal) return QVariant();

  switch (role) {
    case Qt::DisplayRole:
      switch (section) {
        case 0: return QString("time");
        case 1: return QString("note");
      }
      break;
  }

  return QVariant();
}

int DailyTasksProvider::rowCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent);
  return tasks_.size();
}

int DailyTasksProvider::columnCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent);
  return 2;
}

QVariant DailyTasksProvider::data(const QModelIndex& index, int role) const
{
  if (!index.isValid()) return QVariant();

  const TaskPtr& task = tasks_.at(index.row());

  switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
      switch (index.column()) {
        case 0: return task->time();
        case 1: return task->note();
      }
      break;
  }

  return QVariant();
}

bool DailyTasksProvider::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if (data(index, role) != value) {
    const TaskPtr& task = tasks_.at(index.row());

    switch (index.column()) {
      case 0:
        task->setTime(value.toTime());
        break;

      case 1:
        task->setNote(value.toString());
        break;
    }

    emit dataChanged(index, index, QVector<int>() << role);
    return true;
  }
  return false;
}

Qt::ItemFlags DailyTasksProvider::flags(const QModelIndex& index) const
{
  if (!index.isValid()) return Qt::NoItemFlags;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

TaskPtr DailyTasksProvider::getTask(const QModelIndex& index) const
{
  if (!index.isValid()) return TaskPtr();

  return tasks_.at(index.row());
}

void DailyTasksProvider::setTasks(const QList<TaskPtr>& lst)
{
  beginResetModel();
  tasks_ = lst;
  endResetModel();
}

} // namespace schedule
