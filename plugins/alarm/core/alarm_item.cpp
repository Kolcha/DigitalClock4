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

#include "alarm_item.h"

namespace alarm_plugin {

AlarmItem::AlarmItem(QObject* parent) :
  QObject(parent),
  id_(0),
  time_(0, 0),
  volume_(75),
  is_enabled_(false)
{
  days_.insert(Qt::Monday);
  days_.insert(Qt::Tuesday);
  days_.insert(Qt::Wednesday);
  days_.insert(Qt::Thursday);
  days_.insert(Qt::Friday);
  days_.insert(Qt::Saturday);
  days_.insert(Qt::Sunday);

  connect(this, &AlarmItem::timeChanged, this, &AlarmItem::edited);
  connect(this, &AlarmItem::daysChanged, this, &AlarmItem::edited);
  connect(this, &AlarmItem::toggled, this, &AlarmItem::edited);
}

int AlarmItem::id() const
{
  return id_;
}

QTime AlarmItem::time() const
{
  return time_;
}

QUrl AlarmItem::media() const
{
  return media_;
}

int AlarmItem::volume() const
{
  return volume_;
}

QSet<Qt::DayOfWeek> AlarmItem::days() const
{
  return days_;
}

bool AlarmItem::isEnabled() const
{
  return is_enabled_;
}

bool AlarmItem::idCompare(const AlarmItem* a, const AlarmItem* b)
{
  return a->id() < b->id();
}

bool AlarmItem::timeCompare(const AlarmItem* a, const AlarmItem* b)
{
  return a->time() < b->time();
}

void AlarmItem::setId(const int id)
{
  id_ = id;
}

void AlarmItem::setTime(const QTime& tm)
{
  time_ = tm;
  emit timeChanged(time_);
}

void AlarmItem::setMedia(const QUrl& url)
{
  media_ = url;
  emit edited();
}

void AlarmItem::setVolume(int new_volume)
{
  volume_ = new_volume;
  emit edited();
}

void AlarmItem::setDays(const QSet<Qt::DayOfWeek>& new_days)
{
  days_ = new_days;
  emit daysChanged(days_);
}

void AlarmItem::setEnabled(bool enabled)
{
  is_enabled_ = enabled;
  emit toggled(is_enabled_);
}

} // namespace alarm_plugin
