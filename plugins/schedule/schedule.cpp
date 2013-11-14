#include <QMenu>
#include "core/task_manager.h"
#include "schedule.h"

Schedule::Schedule() {
  manager_ = new TaskManager(this);
}

void Schedule::Init(QWidget* main_wnd) {
  parent_ = main_wnd;
  manager_->LoadTasks();
}

void Schedule::Start() {
  tray_icon_ = new QSystemTrayIcon(QIcon(":/schedule.svg"));
  tray_menu_ = new QMenu();
  tray_menu_->addAction(QIcon(":/settings.svg"), "Settings", this, SLOT(Configure()));
  tray_icon_->setContextMenu(tray_menu_);
  tray_icon_->setToolTip("Digital Clock Schedule Plugin");
  tray_icon_->setVisible(true);
  connect(tray_icon_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          this, SLOT(TrayActivated(QSystemTrayIcon::ActivationReason)));
  connect(manager_, SIGNAL(TaskFound(QString)), this, SLOT(ShowMessage(QString)));
}

void Schedule::Stop() {
  tray_icon_->setVisible(false);
  delete tray_icon_;
  delete tray_menu_;
}

void Schedule::Configure() {
  if (settings_dlg_) {
    settings_dlg_->activateWindow();
  } else {
    settings_dlg_ = new SettingsDialog(parent_);
    connect(manager_, SIGNAL(DatesUpdated(QList<QDate>)),
            settings_dlg_, SLOT(SetDates(QList<QDate>)));
    connect(manager_, SIGNAL(TasksUpdated(QMap<QTime,QString>)),
            settings_dlg_, SLOT(SetTasks(QMap<QTime,QString>)));
    connect(settings_dlg_, SIGNAL(TaskAdded(Task)), manager_, SLOT(AddTask(Task)));
    connect(settings_dlg_, SIGNAL(TaskRemoved(Task)), manager_, SLOT(RemoveTask(Task)));
    connect(settings_dlg_, SIGNAL(DateChanged(QDate)), manager_, SLOT(SetCurrentDate(QDate)));
    connect(settings_dlg_, SIGNAL(accepted()), manager_, SLOT(SaveTasks()));
    connect(settings_dlg_, SIGNAL(rejected()), manager_, SLOT(LoadTasks()));
    manager_->LoadTasks();
    settings_dlg_->show();
  }
}

void Schedule::TimeUpdateListener(const QString&) {
  QDateTime now = QDateTime::currentDateTime();
  manager_->CheckTime(now.addSecs(-now.time().second()).addMSecs(-now.time().msec()));
}

void Schedule::TrayActivated(QSystemTrayIcon::ActivationReason reason) {
  if (reason == QSystemTrayIcon::DoubleClick) {
    Configure();
  }
}

void Schedule::ShowMessage(const QString& message) {
  if (!tray_icon_) return;
  tray_icon_->showMessage(tr("Scheduled task"), message);
}
