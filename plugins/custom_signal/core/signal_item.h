/*
    Digital Clock: custom signal plugin
    Copyright (C) 2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef CUSTOM_SIGNAL_SIGNAL_ITEM_H
#define CUSTOM_SIGNAL_SIGNAL_ITEM_H

#include <QObject>

#include <QUrl>
#include <QTime>

namespace custom_signal {

class SignalItem : public QObject
{
  Q_OBJECT
public:
  explicit SignalItem(QObject* parent = 0);

  int period() const;
  int offset() const;

  int volume() const;
  QUrl sound() const;

  QTime next() const;

  bool enabled() const;

signals:
  void timeout();
  void nextChanged(const QTime& next);

public slots:
  void setPeriod(int period);
  void setOffset(int offset);

  void setVolume(int volume);
  void setSound(const QUrl& sound);

  void setEnabled(bool enabled);

  void checkTime(const QTime& t);

private:
  int period_;
  int offset_;
  int volume_;
  QUrl sound_;
  QTime next_shot_;
  bool enabled_;
};

QTime next_time(const QTime& now, const int period, const int offset);

} // namespace custom_signal

#endif // CUSTOM_SIGNAL_SIGNAL_ITEM_H
