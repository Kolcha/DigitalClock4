/*
    Digital Clock: alarm plugin
    Copyright (C) 2013-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef ALARM_PLUGIN_ALARM_H
#define ALARM_PLUGIN_ALARM_H

#include "iclock_plugin.h"
#include "iplugin_init.h"

#include <QIcon>
#include <QPointer>
#include <QMediaPlayer>

class QAction;
class QHotkey;

namespace alarm_plugin {

class AlarmsStorage;

class Alarm : public IClockPlugin, public ITrayPluginInit
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "alarm.json")
  Q_INTERFACES(IClockPlugin ITrayPluginInit)

public:
  Alarm();
  void Init(QSystemTrayIcon* tray_icon) override;

  void InitSettings(SettingsStorage* backend, const QString& name) override;

public slots:
  void Start() override;
  void Stop() override;
  void Configure() override;
  void TimeUpdateListener() override;

private slots:
  void ShowPlayerError(QMediaPlayer::Error error);
  void ShowSettingsDialog();
  void onPluginOptionChanged(const QString& key, const QVariant& value);

private:
  QSystemTrayIcon* tray_icon_;
  QIcon old_icon_;
  QPointer<QMediaPlayer> player_;
  bool icon_changed_;
  AlarmsStorage* storage_;
  QAction* alarm_menu_;
  QHotkey* stop_hotkey_;
};

} // namespace alarm_plugin

#endif // ALARM_PLUGIN_ALARM_H
