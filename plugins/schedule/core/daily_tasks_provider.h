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

#ifndef SCHEDULE_DAILY_TASKS_PROVIDER_H
#define SCHEDULE_DAILY_TASKS_PROVIDER_H

#include <QAbstractTableModel>

#include <QList>

#include "core/task.h"

namespace schedule {

class DailyTasksProvider : public QAbstractTableModel
{
  Q_OBJECT

public:
  explicit DailyTasksProvider(QObject* parent = nullptr);

  // Header:
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

  // Basic functionality:
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  // Editable:
  bool setData(const QModelIndex& index, const QVariant& value,
               int role = Qt::EditRole) override;

  Qt::ItemFlags flags(const QModelIndex& index) const override;

  TaskPtr getTask(const QModelIndex& index) const;

public slots:
  void setTasks(const QList<TaskPtr>& lst);

private:
  QList<TaskPtr> tasks_;
};

} // namespace schedule

#endif // SCHEDULE_DAILY_TASKS_PROVIDER_H
