/*
    Digital Clock: schedule plugin
    Copyright (C) 2017-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef SCHEDULE_TASK_ADVANCED_SETTINGS_DIALOG_H
#define SCHEDULE_TASK_ADVANCED_SETTINGS_DIALOG_H

#include <QDialog>

#include "core/notification.h"

namespace schedule {

namespace Ui {
class TaskAdvancedSettingsDialog;
}

class TaskAdvancedSettingsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit TaskAdvancedSettingsDialog(QWidget* parent = nullptr);
  ~TaskAdvancedSettingsDialog();

  Notification notification() const;
  void setNotification(const Notification& nt);

private slots:
  void on_preview_btn_clicked();

private:
  Ui::TaskAdvancedSettingsDialog* ui;
};

} // namespace schedule

#endif // SCHEDULE_TASK_ADVANCED_SETTINGS_DIALOG_H
