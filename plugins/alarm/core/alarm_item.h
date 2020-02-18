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

#ifndef ALARM_PLUGIN_ALARM_ITEM_H
#define ALARM_PLUGIN_ALARM_ITEM_H

#include <QObject>

#include <QTime>
#include <QSet>
#include <QUrl>

namespace alarm_plugin {

class AlarmItem : public QObject
{
  Q_OBJECT

public:
  explicit AlarmItem(QObject* parent = nullptr);

  int id() const;

  QTime time() const;
  QUrl media() const;
  int volume() const;
  QSet<Qt::DayOfWeek> days() const;
  bool isEnabled() const;

  static bool idCompare(const AlarmItem* a, const AlarmItem* b);
  static bool timeCompare(const AlarmItem* a, const AlarmItem* b);

signals:
  void timeChanged(const QTime& new_time);
  void daysChanged(const QSet<Qt::DayOfWeek>& days);
  void toggled(bool enabled);

  void edited();

public slots:
  void setId(const int id);

  void setTime(const QTime& tm);
  void setMedia(const QUrl& url);
  void setVolume(int new_volume);
  void setDays(const QSet<Qt::DayOfWeek>& new_days);
  void setEnabled(bool enabled);

private:
  int id_;
  QTime time_;
  QUrl media_;
  int volume_;
  QSet<Qt::DayOfWeek> days_;
  bool is_enabled_;
};

} // namespace alarm_plugin

#endif // ALARM_PLUGIN_ALARM_ITEM_H
