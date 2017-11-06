/*
    Digital Clock: alarm plugin
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

#include "alarm.h"

#include <QSystemTrayIcon>
#include <QMenu>
#include <QFile>
#include <QDir>
#include <QMediaPlaylist>
#ifdef HAVE_QHOTKEY
#include <QHotkey>
#endif
#include "plugin_settings.h"

#include "core/alarm_item.h"
#include "core/alarms_storage.h"
#include "core/alarm_settings.h"
#include "gui/alarms_list_dialog.h"
#include "gui/advanced_settings_dialog.h"

namespace alarm_plugin {

Alarm::Alarm() :
  tray_icon_(nullptr),
  icon_changed_(false),
  storage_(nullptr),
  alarm_menu_(nullptr),
  stop_hotkey_(nullptr)
{
  InitTranslator(QLatin1String(":/alarm/alarm_"));
  info_.display_name = tr("Alarm");
  info_.description = tr("Allows to set multiple alarms.");
  InitIcon(":/alarm/alarm_clock.svg.p");
}

void Alarm::Init(QSystemTrayIcon* tray_icon)
{
  tray_icon_ = tray_icon;
  old_icon_ = tray_icon->icon();
}

void Alarm::InitSettings(SettingsStorage* backend)
{
  storage_ = new AlarmsStorage(backend, this);
  IClockPlugin::InitSettings(backend);
}

void Alarm::Start()
{
  QIcon tray_icon(":/alarm/alarm_clock.svg.p");
  tray_icon.setIsMask(true);
  tray_icon_->setIcon(tray_icon);
  icon_changed_ = true;

  player_ = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
  player_->setPlaylist(new QMediaPlaylist(player_));
  player_->playlist()->setPlaybackMode(QMediaPlaylist::Loop);

  QMenu* tray_menu = tray_icon_->contextMenu();
  QMenu* alarm_menu = new QMenu(tr("Alarm"));
  alarm_menu->addAction(tr("Stop Alarm"), player_.data(), &QMediaPlayer::stop);
  alarm_menu->addAction(tr("Configure"), this, &Alarm::Configure);
  alarm_menu_ = tray_menu->insertMenu(tray_menu->actions().first(), alarm_menu);
  alarm_menu_->setIcon(tray_icon_->icon());

  storage_->loadAlarms();

  QList<AlarmItem*> alarms = storage_->getAlarms();
  QStringList bad_files;
  for (auto& alarm : alarms) {
    if (!alarm->media().isValid() || alarm->media().isEmpty()) {
      tray_icon_->showMessage(tr("Digital Clock Alarm"),
                              tr("Invalid media source is set for one of alarms. Click this message to fix."),
                              QSystemTrayIcon::Warning);
      connect(tray_icon_, &QSystemTrayIcon::messageClicked, this, &Alarm::Configure);
      break;
    }

    if (alarm->media().isLocalFile() && !QFile::exists(alarm->media().toLocalFile())) {
      bad_files.append(QDir::toNativeSeparators(alarm->media().toLocalFile()));
    }
  }

  if (!bad_files.isEmpty()) {
    disconnect(tray_icon_, &QSystemTrayIcon::messageClicked, 0, 0);
    tray_icon_->showMessage(tr("Digital Clock Alarm"),
                            tr("Next media files was NOT found:\n%1").arg(bad_files.join('\n')),
                            QSystemTrayIcon::Warning);
    connect(tray_icon_, &QSystemTrayIcon::messageClicked, this, &Alarm::Configure);
  }

  connect(player_.data(), static_cast<void(QMediaPlayer::*)(QMediaPlayer::Error)>(&QMediaPlayer::error),
          this, &Alarm::ShowPlayerError);

  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
  settings_->TrackChanges(true);
  connect(settings_, &PluginSettings::OptionChanged, this, &Alarm::onPluginOptionChanged);
  settings_->Load();
}

void Alarm::Stop()
{
  tray_icon_->setIcon(old_icon_);
  icon_changed_ = false;
  if (player_) {
    if (player_->state() == QMediaPlayer::PlayingState) player_->stop();
    delete player_;
  }
  tray_icon_->contextMenu()->removeAction(alarm_menu_);
  delete alarm_menu_->menu();
  delete stop_hotkey_;
}

void Alarm::Configure()
{
  AlarmsListDialog* dlg = new AlarmsListDialog();

  if (!icon_changed_) {
    QSettings::SettingsMap curr_settings;
    InitDefaults(&curr_settings);
    settings_->SetDefaultValues(curr_settings);
  }
  dlg->setLastMediaPath(settings_->GetOption(OPT_LAST_MEDIA_PATH).toString());
  connect(dlg, &AlarmsListDialog::lastMediaPathChanged, [this] (const QString& last_path) {
    settings_->SetOption(OPT_LAST_MEDIA_PATH, last_path);
  });
  connect(dlg, &AlarmsListDialog::accepted, settings_, &PluginSettings::Save);
  connect(dlg, &AlarmsListDialog::rejected, settings_, &PluginSettings::Load);

  connect(dlg, &AlarmsListDialog::alarmAdded, storage_, &AlarmsStorage::addAlarm);
  connect(dlg, &AlarmsListDialog::alarmRemoved, storage_, &AlarmsStorage::removeAlarm);
  connect(dlg, &AlarmsListDialog::accepted, storage_, &AlarmsStorage::Accept);
  connect(dlg, &AlarmsListDialog::rejected, storage_, &AlarmsStorage::Reject);
  connect(dlg, &AlarmsListDialog::settingsButtonClicked, this, &Alarm::ShowSettingsDialog);

  connect(storage_, &AlarmsStorage::alarmsLoaded, dlg, &AlarmsListDialog::setAlarmsList);
  if (icon_changed_)
    dlg->setAlarmsList(storage_->getAlarms());
  else
    storage_->loadAlarms();
  dlg->show();
}

void Alarm::TimeUpdateListener()
{
  if (!icon_changed_) return;

  QDateTime curr_time = QDateTime::currentDateTime();
  if (curr_time.time().second() != 0) return;

  if (player_->state() == QMediaPlayer::PlayingState) return;

  QList<AlarmItem*> alarms = storage_->getAlarms();
  for (auto& alarm : alarms) {
    if (!alarm->isEnabled()) continue;
    if (!alarm->days().contains(static_cast<Qt::DayOfWeek>(curr_time.date().dayOfWeek()))) continue;
    if (alarm->time().hour() != curr_time.time().hour() ||
        alarm->time().minute() != curr_time.time().minute()) continue;

    disconnect(tray_icon_, &QSystemTrayIcon::messageClicked, 0, 0);
    if (alarm->media().isLocalFile() && !QFile::exists(alarm->media().toLocalFile())) {
      tray_icon_->showMessage(tr("Digital Clock Alarm"),
                              tr("File not found:\n%1").arg(QDir::toNativeSeparators(alarm->media().toLocalFile())),
                              QSystemTrayIcon::Critical);
      continue;
    }

    player_->playlist()->clear();
    player_->playlist()->addMedia(alarm->media());
    player_->setVolume(alarm->volume());
    player_->play();

    tray_icon_->showMessage(tr("Digital Clock Alarm"),
                            tr("Alarm at %1").arg(alarm->time().toString(Qt::SystemLocaleLongDate)),
                            QSystemTrayIcon::Information, 30000);
    connect(tray_icon_, &QSystemTrayIcon::messageClicked, player_.data(), &QMediaPlayer::stop);
  }
}

void Alarm::ShowPlayerError(QMediaPlayer::Error error)
{
  if (error == QMediaPlayer::NoError) return;
  tray_icon_->showMessage(tr("Digital Clock Alarm"), player_->errorString(), QSystemTrayIcon::Critical);
}

void Alarm::ShowSettingsDialog()
{
  AdvancedSettingsDialog* dlg = new AdvancedSettingsDialog(qobject_cast<QWidget*>(sender()));
  dlg->setWindowModality(Qt::ApplicationModal);
  // load current settings to dialog
  QSettings::SettingsMap curr_settings;
  InitDefaults(&curr_settings);
  if (!icon_changed_) settings_->SetDefaultValues(curr_settings);
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

void Alarm::onPluginOptionChanged(const QString& key, const QVariant& value)
{
  if (key == OPT_STOP_ALARM_SHORTCUT) {
    delete stop_hotkey_;
    QString key_seq_str = value.toString();
    if (key_seq_str.isEmpty()) return;
#ifdef HAVE_QHOTKEY
    stop_hotkey_ = new QHotkey(QKeySequence(key_seq_str), true);
    connect(stop_hotkey_, &QHotkey::activated, player_.data(), &QMediaPlayer::stop);
#endif
    return;
  }
}

} // namespace alarm_plugin
