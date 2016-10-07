/*
    Digital Clock: schedule plugin
    Copyright (C) 2013-2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef SCHEDULE_SCHEDULE_DIALOG_H
#define SCHEDULE_SCHEDULE_DIALOG_H

#include <QDialog>

#include <QList>

#include "core/task.h"

class QSortFilterProxyModel;

namespace schedule {

class DailyTasksProvider;

namespace Ui {
class ScheduleDialog;
}

class ScheduleDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ScheduleDialog(QWidget* parent = 0);
  ~ScheduleDialog();

signals:
  void dateChanged(const QDate& dt);
  void taskCreated(const TaskPtr& tsk);
  void taskDeleted(const TaskPtr& tsk);

public slots:
  void setDates(const QList<QDate>& dates);
  void setTasks(const QList<TaskPtr>& tasks);

private slots:
  void on_dates_box_currentIndexChanged(int index);
  void on_add_btn_clicked();
  void on_del_btn_clicked();

  void on_edit_btn_clicked();

private:
  Ui::ScheduleDialog* ui;
  DailyTasksProvider* tasks_model_;
  QSortFilterProxyModel* sort_model_;
};

} // namespace schedule

#endif // SCHEDULE_SCHEDULE_DIALOG_H
