#include <QMenu>
#include "plugin_settings.h"
#include "core/schedule_settings.h"
#include "core/task_manager.h"
#include "schedule.h"

namespace schedule {

Schedule::Schedule() {
  settings_ = new PluginSettings(ORG_NAME, APP_NAME, this);
  manager_ = new TaskManager(this);

  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);

  settings_->Load();
  manager_->LoadTasks();

  InitTranslator(QLatin1String(":/schedule/schedule_"));
  info_.display_name = tr("Scheduler");
  info_.description = tr("Allow to schedule a task and display notification at specified time.");
  InitIcon(":/schedule/schedule.svg");
}

void Schedule::ExportSettings(QSettings::SettingsMap* settings) {
  if (!settings) return;
  *settings = settings_->GetSettingsMap();
}

void Schedule::ImportSettings(const QSettings::SettingsMap& settings) {
  settings_->SetValues(settings);
  settings_->Save();
}


void Schedule::Start() {
  tray_icon_ = new QSystemTrayIcon(QIcon(":/schedule/schedule.svg"));
  tray_menu_ = new QMenu();
  tray_menu_->addAction(QIcon(":/schedule/settings.svg"), "Settings", this, SLOT(Configure()));
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
    settings_dlg_ = new SettingsDialog();
    // load current settings to dialog
    connect(settings_, SIGNAL(OptionChanged(QString,QVariant)),
            settings_dlg_, SLOT(SettingsListener(QString,QVariant)));
    settings_->TrackChanges(true);
    settings_->Load();
    settings_->TrackChanges(false);
    // connect main signals/slots
    connect(manager_, SIGNAL(DatesUpdated(QList<QDate>)),
            settings_dlg_, SLOT(SetDates(QList<QDate>)));
    connect(manager_, SIGNAL(TasksUpdated(QMap<QTime,QString>)),
            settings_dlg_, SLOT(SetTasks(QMap<QTime,QString>)));
    connect(settings_dlg_, SIGNAL(TaskAdded(Task)), manager_, SLOT(AddTask(Task)));
    connect(settings_dlg_, SIGNAL(TaskRemoved(Task)), manager_, SLOT(RemoveTask(Task)));
    connect(settings_dlg_, SIGNAL(DateChanged(QDate)), manager_, SLOT(SetCurrentDate(QDate)));
    connect(settings_dlg_, SIGNAL(accepted()), manager_, SLOT(SaveTasks()));
    connect(settings_dlg_, SIGNAL(rejected()), manager_, SLOT(LoadTasks()));
    connect(settings_dlg_, SIGNAL(OptionChanged(QString,QVariant)),
            settings_, SLOT(SetOption(QString,QVariant)));
    connect(settings_dlg_, SIGNAL(accepted()), settings_, SLOT(Save()));
    connect(settings_dlg_, SIGNAL(rejected()), settings_, SLOT(Load()));
    manager_->LoadTasks();
    settings_dlg_->show();
  }
}

void Schedule::TimeUpdateListener() {
  QDateTime now = QDateTime::currentDateTime();
  manager_->CheckTime(now.addSecs(-now.time().second()).addMSecs(-now.time().msec()),
                      settings_->GetOption(OPT_TASK_DELETE).toBool());
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

} // namespace schedule
