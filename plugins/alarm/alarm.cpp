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

#include "core/alarm_item.h"
#include "core/alarms_storage.h"
#include "gui/settings_dialog.h"

namespace alarm_plugin {

Alarm::Alarm() : tray_icon_(nullptr), icon_changed_(false), storage_(nullptr), alarm_menu_(nullptr)
{
  InitTranslator(QLatin1String(":/alarm/alarm_"));
  info_.display_name = tr("Alarm");
  info_.description = tr("Allows to set multiple alarms.");
  InitIcon(":/alarm/alarm_clock.svg");
}

void Alarm::Init(QSystemTrayIcon* tray_icon)
{
  tray_icon_ = tray_icon;
  old_icon_ = tray_icon->icon();
}

void Alarm::InitSettings(SettingsStorage* backend)
{
  storage_ = new AlarmsStorage(backend, this);
}

void Alarm::Start()
{
  tray_icon_->setIcon(QIcon(":/alarm/alarm_clock.svg"));
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
                              QSystemTrayIcon::Warning, 15000);
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
                            QSystemTrayIcon::Warning, 15000);
    connect(tray_icon_, &QSystemTrayIcon::messageClicked, this, &Alarm::Configure);
  }

  connect(player_.data(), static_cast<void(QMediaPlayer::*)(QMediaPlayer::Error)>(&QMediaPlayer::error),
          this, &Alarm::ShowPlayerError);
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
}

void Alarm::Configure()
{
  SettingsDialog* dlg = new SettingsDialog();
  connect(dlg, &SettingsDialog::alarmAdded, storage_, &AlarmsStorage::addAlarm);
  connect(dlg, &SettingsDialog::alarmRemoved, storage_, &AlarmsStorage::removeAlarm);
  connect(dlg, &SettingsDialog::accepted, storage_, &AlarmsStorage::Accept);
  connect(dlg, &SettingsDialog::rejected, storage_, &AlarmsStorage::Reject);

  connect(storage_, &AlarmsStorage::alarmsLoaded, dlg, &SettingsDialog::setAlarmsList);
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
                              QSystemTrayIcon::Critical, 20000);
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

} // namespace alarm_plugin
