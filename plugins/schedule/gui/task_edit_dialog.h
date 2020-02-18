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

#ifndef SCHEDULE_TASK_EDIT_DIALOG_H
#define SCHEDULE_TASK_EDIT_DIALOG_H

#include <QDialog>

#include "core/notification.h"

namespace schedule {

namespace Ui {
class TaskEditDialog;
}

class TaskEditDialog : public QDialog
{
  Q_OBJECT

public:
  explicit TaskEditDialog(QWidget* parent = nullptr);
  ~TaskEditDialog();

  QDate date() const;
  QTime time() const;

  QString note() const;

  Notification notification() const;


  void setDate(const QDate& dt);
  void setTime(const QTime& tm);

  void setNote(const QString& nt);

  void setNotification(const Notification& nt);

private slots:
  void on_adv_settings_btn_clicked();

private:
  Ui::TaskEditDialog* ui;
  Notification notification_;
};

} // namespace schedule

#endif // SCHEDULE_TASK_EDIT_DIALOG_H
