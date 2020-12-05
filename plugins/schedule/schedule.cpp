/*
    Digital Clock: schedule plugin
    Copyright (C) 2013-2020  Nick Korotysh <nick.korotysh@gmail.com>

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

#include <QFile>
#include <QMenu>
#include <QTimer>
#include <QMediaPlayer>

#ifdef HAVE_QHOTKEY
#include <QHotkey>
#else
class QHotkey {};   // just a stub to suppress compiler warnings
#endif

#include "message_box.h"
#include "plugin_settings.h"

#include "core/schedule_settings.h"
#include "core/tasks_storage.h"
#include "core/tasks_invoker.h"

#include "gui/advanced_settings_dialog.h"
#include "gui/schedule_dialog.h"
#include "gui/task_edit_dialog.h"

namespace schedule {

Schedule::Schedule() : tray_menu_(nullptr), backend_(nullptr), invoker_(nullptr), player_(nullptr)
{
  InitTranslator(QLatin1String(":/schedule/lang/schedule_"));
  info_.display_name = tr("Scheduler");
  info_.description = tr("Allow to schedule a task and display notification at specified time.");
  InitIcon(":/schedule/schedule.svg");
}

void Schedule::Init(QSystemTrayIcon* tray_icon)
{
  clock_icon_ = tray_icon;
}

void Schedule::InitSettings(SettingsStorage* backend, const QString& name)
{
  backend_ = new TasksStorage(backend, this);
  IClockPlugin::InitSettings(backend, name);
}

void Schedule::Start()
{
  player_ = new QMediaPlayer();
  tray_icon_ = new QSystemTrayIcon(QIcon(":/schedule/schedule.svg"));
  tray_menu_ = new QMenu();
  tray_menu_->addAction(QIcon(":/schedule/settings.svg.p"), "Settings", this, SLOT(Configure()));
  tray_icon_->setContextMenu(tray_menu_);
  tray_icon_->setToolTip("Digital Clock Schedule Plugin");
  tray_icon_->setVisible(true);
  connect(tray_icon_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          this, SLOT(TrayActivated(QSystemTrayIcon::ActivationReason)));
  connect(tray_icon_, &QSystemTrayIcon::messageClicked, player_, &QMediaPlayer::stop);

  invoker_ = new TasksInvoker(this);
  invoker_->useExternalTimer(true);

  connect(backend_, &TasksStorage::tasksLoaded, invoker_, &TasksInvoker::setDailyTasks);
  connect(invoker_, &TasksInvoker::dateChanged, backend_, &TasksStorage::LoadTasks);
  connect(invoker_, &TasksInvoker::completed, this, &Schedule::TaskCompleted);
  connect(invoker_, &TasksInvoker::completed, backend_, &TasksStorage::delTask);
  connect(invoker_, &TasksInvoker::done, backend_, &TasksStorage::Accept);

  invoker_->start();

  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
  settings_->TrackChanges(true);
  connect(settings_, &PluginSettings::OptionChanged, this, &Schedule::onPluginOptionChanged);
  settings_->Load();
}

void Schedule::Stop()
{
  invoker_->stop();

  tray_icon_->setVisible(false);
  delete tray_icon_;
  delete tray_menu_;

  player_->stop();
  delete player_;

  delete add_task_hotkey_;
}

void Schedule::Configure()
{
  ScheduleDialog* dlg = new ScheduleDialog();
  connect(dlg, &ScheduleDialog::destroyed, this, &Schedule::configured);
  connect(backend_, &TasksStorage::datesLoaded, dlg, &ScheduleDialog::setDates);
  connect(backend_, &TasksStorage::tasksLoaded, dlg, &ScheduleDialog::setTasks);
  connect(dlg, &ScheduleDialog::dateChanged, backend_, &TasksStorage::LoadTasks);
  connect(dlg, &ScheduleDialog::taskCreated, backend_, &TasksStorage::addTask);
  connect(dlg, &ScheduleDialog::taskDeleted, backend_, &TasksStorage::delTask);
  connect(dlg, &ScheduleDialog::taskEdited, backend_, &TasksStorage::updateTask);
  connect(dlg, &ScheduleDialog::accepted, backend_, &TasksStorage::Accept);
  connect(dlg, &ScheduleDialog::rejected, backend_, &TasksStorage::Reject);

  connect(dlg, &ScheduleDialog::accepted, dlg, &ScheduleDialog::deleteLater);
  connect(dlg, &ScheduleDialog::rejected, dlg, &ScheduleDialog::deleteLater);

  connect(this, &Schedule::defaultNotificationChanged, dlg, &ScheduleDialog::setDefaultNotification);
  connect(dlg, &ScheduleDialog::settingsButtonClicked, this, &Schedule::ShowSettingsDialog);

  Notification nt;
  nt.setType(settings_->GetOption(OPT_NOTIFICATION_TYPE).value<Notification::Type>());
  nt.setTimeout(settings_->GetOption(OPT_NOTIFICATION_TIME).toInt());
  nt.setPlaySound(settings_->GetOption(OPT_PLAY_SOUND).toBool());
  nt.setSoundFile(settings_->GetOption(OPT_SOUND_FILE).toString());
  dlg->setDefaultNotification(nt);

  if (invoker_) {
    invoker_->stop();
    connect(dlg, &ScheduleDialog::destroyed, invoker_, &TasksInvoker::start);
  }

  backend_->loadDates();
  dlg->setModal(true);
  dlg->show();
}

void Schedule::TimeUpdateListener()
{
  if (invoker_) invoker_->externalTimerHandler();
}

void Schedule::TrayActivated(QSystemTrayIcon::ActivationReason reason)
{
  if (reason == QSystemTrayIcon::DoubleClick) {
    Configure();
  }
}

void Schedule::TaskCompleted(const TaskPtr& task)
{
  if (task->notification().playSound() && QFile::exists(task->notification().soundFile())) {
    player_->setMedia(QUrl::fromLocalFile(task->notification().soundFile()));
    player_->play();
  }

  switch (task->notification().type()) {
    case Notification::TrayMessage: {
      if (!tray_icon_) return;
      auto tray_icon = tray_icon_->isVisible() ? tray_icon_.data() : clock_icon_;
      if (!tray_icon) return;
      tray_icon->showMessage(tr("Scheduled task"), task->note(),
                              QSystemTrayIcon::Information,
                              task->notification().timeout() * 1000);
      break;
    }

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
      player_->stop();
      break;

    default:
      break;
  }

  if (player_->state() == QMediaPlayer::PlayingState && task->notification().timeout() > 0) {
    QTimer* timer = new QTimer(this);
    timer->setInterval(task->notification().timeout() * 1000 + 500);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, player_, &QMediaPlayer::stop);
    connect(timer, &QTimer::timeout, timer, &QTimer::deleteLater);
    timer->start();
  }
}

void Schedule::ShowSettingsDialog()
{
  AdvancedSettingsDialog* dlg = new AdvancedSettingsDialog(qobject_cast<QWidget*>(sender()));
  dlg->setWindowModality(Qt::ApplicationModal);
  // load current settings to dialog
  QSettings::SettingsMap curr_settings;
  InitDefaults(&curr_settings);
  if (!player_) settings_->SetDefaultValues(curr_settings);
  for (auto iter = curr_settings.begin(); iter != curr_settings.end(); ++iter) {
    *iter = settings_->GetOption(iter.key());
  }
  dlg->Init(curr_settings);
  // connect main signals/slots
  connect(dlg, &AdvancedSettingsDialog::accepted, settings_, &PluginSettings::Save);
  connect(dlg, &AdvancedSettingsDialog::rejected, settings_, &PluginSettings::Load);
  connect(dlg, &AdvancedSettingsDialog::OptionChanged, settings_, &PluginSettings::SetOption);
  connect(dlg, &AdvancedSettingsDialog::accepted, dlg, &AdvancedSettingsDialog::deleteLater);
  connect(dlg, &AdvancedSettingsDialog::rejected, dlg, &AdvancedSettingsDialog::deleteLater);
  dlg->show();
}

void Schedule::onPluginOptionChanged(const QString& key, const QVariant& value)
{
  if (key == OPT_NOTIFICATION_TYPE) {
    default_notification_.setType(value.value<Notification::Type>());
    emit defaultNotificationChanged(default_notification_);
  }

  if (key == OPT_NOTIFICATION_TIME) {
    default_notification_.setTimeout(value.toInt());
    emit defaultNotificationChanged(default_notification_);
  }

  if (key == OPT_PLAY_SOUND) {
    default_notification_.setPlaySound(value.toBool());
    emit defaultNotificationChanged(default_notification_);
  }

  if (key == OPT_SOUND_FILE) {
    default_notification_.setSoundFile(value.toString());
    emit defaultNotificationChanged(default_notification_);
  }

  if (key == OPT_SHOW_TRAY_ICON)
    tray_icon_->setVisible(value.toBool());

  auto init_hotkey = [=](auto key_seq, auto receiver, auto method) -> QHotkey* {
    QHotkey* hotkey = nullptr;
#ifdef HAVE_QHOTKEY
    if (!key_seq.isEmpty()) {
      hotkey = new QHotkey(QKeySequence(key_seq), true);
      connect(hotkey, &QHotkey::activated, receiver, method);
    }
#else
    Q_UNUSED(key_seq);
    Q_UNUSED(receiver);
    Q_UNUSED(method);
#endif
    return hotkey;
  };

  if (key == OPT_ADD_TASK_SHORTCUT_E) {
    delete add_task_hotkey_;
    add_task_hotkey_ = nullptr;
    if (value.toBool())
      add_task_hotkey_ = init_hotkey(settings_->GetOption(OPT_ADD_TASK_SHORTCUT).toString(), this, &Schedule::AddTask);
  }

  if (key == OPT_ADD_TASK_SHORTCUT) {
    delete add_task_hotkey_;
    add_task_hotkey_ = nullptr;
    if (settings_->GetOption(OPT_ADD_TASK_SHORTCUT_E).toBool())
      add_task_hotkey_ = init_hotkey(value.toString(), this, &Schedule::AddTask);
  }
}

void Schedule::AddTask()
{
  TaskEditDialog dlg;
  dlg.setDate(QDate::currentDate());
  dlg.setNotification(default_notification_);
  dlg.setWindowModality(Qt::ApplicationModal);
  if (dlg.exec() == QDialog::Accepted) {
    TaskPtr task(new Task());
    task->setDate(dlg.date());
    task->setTime(dlg.time());
    task->setNote(dlg.note());
    task->setNotification(dlg.notification());
    backend_->addTask(task);
    backend_->LoadTasks(QDate::currentDate());
  }
}

} // namespace schedule
