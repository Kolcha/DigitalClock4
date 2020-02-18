/*
    Digital Clock: alarm plugin
    Copyright (C) 2017-2020  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "alarms_storage.h"

#include <algorithm>

#include "settings_storage.h"

#include "alarm_item.h"

namespace alarm_plugin {

static int day2index(Qt::DayOfWeek d)
{
  switch (d) {
    case Qt::Monday:    return 0;
    case Qt::Tuesday:   return 1;
    case Qt::Wednesday: return 2;
    case Qt::Thursday:  return 3;
    case Qt::Friday:    return 4;
    case Qt::Saturday:  return 5;
    case Qt::Sunday:    return 6;
  }
  return 8;
}

static Qt::DayOfWeek index2day(int i)
{
  switch (i) {
    case 0: return Qt::Monday;
    case 1: return Qt::Tuesday;
    case 2: return Qt::Wednesday;
    case 3: return Qt::Thursday;
    case 4: return Qt::Friday;
    case 5: return Qt::Saturday;
    case 6: return Qt::Sunday;
  }
  Q_ASSERT(false);
  return Qt::Sunday;
}

static int encode_days(const QSet<Qt::DayOfWeek>& days)
{
  int res = 0;
  for (auto& day : qAsConst(days)) res |= (1 << day2index(day));
  return res;
}

static QSet<Qt::DayOfWeek> decode_days(int encoded)
{
  QSet<Qt::DayOfWeek> days;
  for (int i = 0; i < 7; ++i) {
    if ((encoded & (1 << i)) != 0)
      days.insert(index2day(i));
  }
  return days;
}


AlarmsStorage::AlarmsStorage(SettingsStorage* backend, QObject* parent) :
  SettingsStorageWrapper(backend, parent),
  key_prefix_("plugins/alarm/alarms")
{
  connect(backend, &SettingsStorage::reloaded, this, &AlarmsStorage::loadAlarms);
}

QList<AlarmItem*> AlarmsStorage::getAlarms() const
{
  return alarms_;
}

void AlarmsStorage::setAlarms(const QList<AlarmItem*>& alarms)
{
  for (auto iter = alarms_.begin(); iter != alarms_.end();) {
    if (!alarms.contains(*iter)) {
      AlarmItem* item = *iter;
      alarms_.erase(iter++);
      item->setParent(nullptr);
      delete item;
    } else {
      ++iter;
    }
  }

  auto max_id_iter = std::max_element(alarms_.begin(), alarms_.end(), AlarmItem::idCompare);
  int id_c = alarms_.isEmpty() ? 1 : (*max_id_iter)->id() + 1;
  for (auto& item : qAsConst(alarms)) {
    if (!alarms_.contains(item)) {
      item->setParent(this);
      item->setId(id_c++);
      alarms_.append(item);
    }
    Q_ASSERT(item->parent() == this);
  }

  std::sort(alarms_.begin(), alarms_.end(), AlarmItem::timeCompare);
  writeAlarms();
}

void AlarmsStorage::loadAlarms()
{
  readAlarms();
  emit alarmsLoaded(alarms_);
}

void AlarmsStorage::addAlarm(AlarmItem* alarm)
{
  Q_ASSERT(!alarms_.contains(alarm));
  alarm->setParent(this);
  auto max_id_iter = std::max_element(alarms_.begin(), alarms_.end(), AlarmItem::idCompare);
  alarm->setId(alarms_.isEmpty() ? 1 : (*max_id_iter)->id() + 1);
  connect(alarm, &AlarmItem::edited, this, &AlarmsStorage::onAlarmEdited);
  alarms_.append(alarm);
  writeItem(alarm);
}

void AlarmsStorage::removeAlarm(AlarmItem* alarm)
{
  Q_ASSERT(alarms_.contains(alarm));
  alarms_.removeOne(alarm);
  Q_ASSERT(!alarms_.contains(alarm));
  QString alarm_key = QString("%1/%2").arg(key_prefix_).arg(alarm->id());
  this->remove(alarm_key);
  alarm->setParent(nullptr);
  delete alarm;
}

void AlarmsStorage::onAlarmEdited()
{
  AlarmItem* item = qobject_cast<AlarmItem*>(sender());
  Q_ASSERT(item);
  writeItem(item);
}

void AlarmsStorage::writeAlarms()
{
  this->remove(key_prefix_);
  for (auto& item : qAsConst(alarms_)) {
    writeItem(item);
  }
}

void AlarmsStorage::readAlarms()
{
  qDeleteAll(alarms_);
  alarms_.clear();
  QStringList alarms_keys = this->GetBackend()->ListChildren(key_prefix_);
  for (auto& key : qAsConst(alarms_keys)) {
    bool id_ok = false;
    int id = key.toInt(&id_ok);
    if (!id_ok) continue;
    AlarmItem* alarm = readItem(id);
    Q_ASSERT(alarm);
    connect(alarm, &AlarmItem::edited, this, &AlarmsStorage::onAlarmEdited);
    alarms_.append(alarm);
  }
}

void AlarmsStorage::writeItem(const AlarmItem* item)
{
  QString alarm_key = QString("%1/%2").arg(key_prefix_).arg(item->id());
  this->setValue(QString("%1/%2").arg(alarm_key, "time"), item->time());
  this->setValue(QString("%1/%2").arg(alarm_key, "days"), encode_days(item->days()));
  this->setValue(QString("%1/%2").arg(alarm_key, "enabled"), item->isEnabled());
  this->setValue(QString("%1/%2").arg(alarm_key, "media"), item->media());
  this->setValue(QString("%1/%2").arg(alarm_key, "volume"), item->volume());
}

AlarmItem* AlarmsStorage::readItem(const int id)
{
  QString alarm_key = QString("%1/%2").arg(key_prefix_).arg(id);
  AlarmItem* item = new AlarmItem(this);
  item->setId(id);
  item->setTime(this->getValue(QString("%1/%2").arg(alarm_key, "time")).toTime());
  item->setDays(decode_days(this->getValue(QString("%1/%2").arg(alarm_key, "days")).toInt()));
  item->setEnabled(this->getValue(QString("%1/%2").arg(alarm_key, "enabled")).toBool());
  item->setMedia(this->getValue(QString("%1/%2").arg(alarm_key, "media")).toUrl());
  item->setVolume(this->getValue(QString("%1/%2").arg(alarm_key, "volume")).toInt());
  return item;
}

} // namespace alarm_plugin
