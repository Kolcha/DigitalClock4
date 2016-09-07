#include "schedule.h"

#include <QMenu>

#include "core/tasks_storage.h"
#include "core/tasks_invoker.h"

#include "gui/schedule_dialog.h"

namespace schedule {

Schedule::Schedule()
{
  invoker_ = nullptr;

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
  tray_menu_->addAction(QIcon(":/schedule/settings.svg"), "Settings", this, SLOT(Configure()));
  tray_icon_->setContextMenu(tray_menu_);
  tray_icon_->setToolTip("Digital Clock Schedule Plugin");
  tray_icon_->setVisible(true);
  connect(tray_icon_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          this, SLOT(TrayActivated(QSystemTrayIcon::ActivationReason)));

  invoker_ = new TasksInvoker(this);

  connect(backend_, &TasksStorage::tasksLoaded, invoker_, &TasksInvoker::setDailyTasks);
  connect(invoker_, &TasksInvoker::dateChanged, backend_, &TasksStorage::LoadTasks);
  connect(invoker_, &TasksInvoker::completed, backend_, &TasksStorage::delTask);
  connect(invoker_, &TasksInvoker::done, backend_, &TasksStorage::commit);
  connect(invoker_, &TasksInvoker::completed, this, &Schedule::TaskCompleted);

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
  connect(dlg, &ScheduleDialog::accepted, backend_, &TasksStorage::commit);
  connect(dlg, &ScheduleDialog::rejected, backend_, &TasksStorage::reject);

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
  if (!tray_icon_) return;
  tray_icon_->showMessage(tr("Scheduled task"), task->note());
}

} // namespace schedule
