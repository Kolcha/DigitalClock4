/*
    Digital Clock: schedule plugin
    Copyright (C) 2013-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "schedule.h"

#include <QMenu>

#include "message_box.h"

#include "core/tasks_storage.h"
#include "core/tasks_invoker.h"

#include "gui/schedule_dialog.h"

namespace schedule {

Schedule::Schedule() : tray_menu_(nullptr), backend_(nullptr), invoker_(nullptr)
{
  InitTranslator(QLatin1String(":/schedule/schedule_"));
  info_.display_name = tr("Scheduler");
  info_.description = tr("Allow to schedule a task and display notification at specified time.");
  InitIcon(":/schedule/schedule.svg");
}

void Schedule::InitSettings(SettingsStorage* backend)
{
  backend_ = new TasksStorage(backend, this);
}

void Schedule::Start()
{
  tray_icon_ = new QSystemTrayIcon(QIcon(":/schedule/schedule.svg"));
  tray_menu_ = new QMenu();
  tray_menu_->addAction(QIcon(":/schedule/settings.svg.p"), "Settings", this, SLOT(Configure()));
  tray_icon_->setContextMenu(tray_menu_);
  tray_icon_->setToolTip("Digital Clock Schedule Plugin");
  tray_icon_->setVisible(true);
  connect(tray_icon_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          this, SLOT(TrayActivated(QSystemTrayIcon::ActivationReason)));

  invoker_ = new TasksInvoker(this);

  connect(backend_, &TasksStorage::tasksLoaded, invoker_, &TasksInvoker::setDailyTasks);
  connect(invoker_, &TasksInvoker::dateChanged, backend_, &TasksStorage::LoadTasks);
  connect(invoker_, &TasksInvoker::completed, this, &Schedule::TaskCompleted);
  connect(invoker_, &TasksInvoker::completed, backend_, &TasksStorage::delTask);
  connect(invoker_, &TasksInvoker::done, backend_, &TasksStorage::Accept);

  invoker_->start();
}

void Schedule::Stop()
{
  invoker_->stop();

  tray_icon_->setVisible(false);
  delete tray_icon_;
  delete tray_menu_;
}

void Schedule::Configure()
{
  ScheduleDialog* dlg = new ScheduleDialog();
  connect(backend_, &TasksStorage::datesLoaded, dlg, &ScheduleDialog::setDates);
  connect(backend_, &TasksStorage::tasksLoaded, dlg, &ScheduleDialog::setTasks);
  connect(dlg, &ScheduleDialog::dateChanged, backend_, &TasksStorage::LoadTasks);
  connect(dlg, &ScheduleDialog::taskCreated, backend_, &TasksStorage::addTask);
  connect(dlg, &ScheduleDialog::taskDeleted, backend_, &TasksStorage::delTask);
  connect(dlg, &ScheduleDialog::accepted, backend_, &TasksStorage::Accept);
  connect(dlg, &ScheduleDialog::rejected, backend_, &TasksStorage::Reject);

  connect(dlg, &ScheduleDialog::accepted, dlg, &ScheduleDialog::deleteLater);
  connect(dlg, &ScheduleDialog::rejected, dlg, &ScheduleDialog::deleteLater);

  if (invoker_) {
    invoker_->stop();
    connect(dlg, &ScheduleDialog::destroyed, invoker_, &TasksInvoker::start);
  }

  backend_->loadDates();
  dlg->setModal(true);
  dlg->show();
}

void Schedule::TrayActivated(QSystemTrayIcon::ActivationReason reason)
{
  if (reason == QSystemTrayIcon::DoubleClick) {
    Configure();
  }
}

void Schedule::TaskCompleted(const TaskPtr& task)
{
  switch (task->notification().type()) {
    case Notification::TrayMessage:
      if (!tray_icon_) return;
      tray_icon_->showMessage(tr("Scheduled task"), task->note(),
                              QSystemTrayIcon::Information,
                              task->notification().timeout() * 1000);
      break;

    case Notification::MessageBox:
      if (task->notification().timeout() > 0) {
        TMessageBox dlg(QMessageBox::Information, tr("Scheduled task"), task->note(), QMessageBox::Ok);
        dlg.setTimeout(task->notification().timeout());
        dlg.setAutoClose(true);
        dlg.setDefaultButton(QMessageBox::Ok);
        dlg.exec();
      } else {
        QMessageBox::information(nullptr, tr("Scheduled task"), task->note(), QMessageBox::Ok);
      }
      break;
  }
}

} // namespace schedule
