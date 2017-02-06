/*
    Digital Clock: alarm plugin
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

#ifndef ALARM_PLUGIN_ALARMS_STORAGE_H
#define ALARM_PLUGIN_ALARMS_STORAGE_H

#include "settings_storage_wrapper.h"

namespace alarm_plugin {

class AlarmItem;

class AlarmsStorage : public SettingsStorageWrapper
{
  Q_OBJECT
public:
  explicit AlarmsStorage(SettingsStorage* backend, QObject* parent = nullptr);

  QList<AlarmItem*> getAlarms();

signals:
  void alarmsLoaded(const QList<AlarmItem*>& alarms);

public slots:
  void setAlarms(const QList<AlarmItem*>& alarms);
  void loadAlarms();

  void addAlarm(AlarmItem* alarm);
  void removeAlarm(AlarmItem* alarm);

private:
  void writeAlarms();
  void readAlarms();

  QList<AlarmItem*> alarms_;
};

} // namespace alarm_plugin

#endif // ALARM_PLUGIN_ALARMS_STORAGE_H
