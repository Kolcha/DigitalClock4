/*
    Digital Clock: schedule plugin
    Copyright (C) 2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "task_advanced_settings_dialog.h"
#include "ui_task_advanced_settings_dialog.h"

#include <QSystemTrayIcon>
#include <QTimer>

#include "message_box.h"

namespace schedule {

TaskAdvancedSettingsDialog::TaskAdvancedSettingsDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::TaskAdvancedSettingsDialog)
{
  ui->setupUi(this);
}

TaskAdvancedSettingsDialog::~TaskAdvancedSettingsDialog()
{
  delete ui;
}

Notification TaskAdvancedSettingsDialog::notification() const
{
  Notification nt;
  if (ui->msg_balloon_rbtn->isChecked()) nt.setType(Notification::TrayMessage);
  if (ui->msg_dialog_rbtn->isChecked()) nt.setType(Notification::MessageBox);
  nt.setTimeout(ui->timeout_edit->value());
  return nt;
}

void TaskAdvancedSettingsDialog::setNotification(const Notification& nt)
{
  ui->msg_balloon_rbtn->setChecked(nt.type() == Notification::TrayMessage);
  ui->msg_dialog_rbtn->setChecked(nt.type() == Notification::MessageBox);
  ui->timeout_edit->setValue(nt.timeout());
}

void TaskAdvancedSettingsDialog::on_preview_btn_clicked()
{
  QString task_text = tr("This is sample task text. Just to test how notification looks.");
  int task_timeout = ui->timeout_edit->value();

  if (ui->msg_balloon_rbtn->isChecked()) {
    QSystemTrayIcon* tray_icon = new QSystemTrayIcon(QIcon(":/schedule/schedule.svg"), this);
    QTimer* timer = new QTimer(this);
    timer->setInterval(task_timeout * 1000 + 500);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, tray_icon, &QSystemTrayIcon::hide);
    connect(timer, &QTimer::timeout, tray_icon, &QSystemTrayIcon::deleteLater);
    connect(timer, &QTimer::timeout, timer, &QTimer::deleteLater);
    timer->start();
    tray_icon->show();
    tray_icon->showMessage(tr("Task preview"), task_text, QSystemTrayIcon::Information, task_timeout * 1000);
  }

  if (ui->msg_dialog_rbtn->isChecked()) {
    if (task_timeout > 0) {
      TMessageBox dlg(QMessageBox::Information, tr("Task preview"), task_text, QMessageBox::Ok);
      dlg.setTimeout(task_timeout);
      dlg.setAutoClose(true);
      dlg.setDefaultButton(QMessageBox::Ok);
      dlg.exec();
    } else {
      QMessageBox::information(nullptr, tr("Task preview"), task_text, QMessageBox::Ok);
    }
  }
}

} // namespace schedule
