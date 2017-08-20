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

#include "task_edit_dialog.h"
#include "ui_task_edit_dialog.h"

#include <QLocale>

namespace schedule {

TaskEditDialog::TaskEditDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::TaskEditDialog)
{
  ui->setupUi(this);

  ui->dateEdit->setDisplayFormat(QLocale::system().dateFormat(QLocale::LongFormat));
  ui->timeEdit->setDisplayFormat(QLocale::system().timeFormat(QLocale::ShortFormat));

  QDateTime now = QDateTime::currentDateTime();
  ui->dateEdit->setMinimumDate(now.date());
  ui->timeEdit->setMinimumTime(now.time());
}

TaskEditDialog::~TaskEditDialog()
{
  delete ui;
}

QDate TaskEditDialog::date() const
{
  return ui->dateEdit->date();
}

QTime TaskEditDialog::time() const
{
  return ui->timeEdit->time();
}

QString TaskEditDialog::note() const
{
  return ui->textEdit->toPlainText();
}

Notification TaskEditDialog::notification() const
{
  return notification_;
}

void TaskEditDialog::setDate(const QDate& dt)
{
  ui->dateEdit->setDate(dt);
}

void TaskEditDialog::setTime(const QTime& tm)
{
  ui->timeEdit->setTime(tm);
}

void TaskEditDialog::setNote(const QString& nt)
{
  ui->textEdit->setPlainText(nt);
}

void TaskEditDialog::setNotification(const Notification& nt)
{
  ui->msg_balloon_rbtn->setChecked(nt.type() == Notification::TrayMessage);
  ui->msg_dialog_rbtn->setChecked(nt.type() == Notification::MessageBox);
  ui->timeout_edit->setValue(nt.timeout());
  notification_ = nt;
}

void TaskEditDialog::on_msg_balloon_rbtn_clicked()
{
  notification_.setType(Notification::TrayMessage);
}

void TaskEditDialog::on_msg_dialog_rbtn_clicked()
{
  notification_.setType(Notification::MessageBox);
}

void TaskEditDialog::on_timeout_edit_valueChanged(int arg1)
{
  notification_.setTimeout(arg1);
}

} // namespace schedule
