/*
    Digital Clock: countdown timer plugin
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

#ifndef COUNTDOWN_TIMER_COUNTDOWN_TIMER_H
#define COUNTDOWN_TIMER_COUNTDOWN_TIMER_H

#include <QObject>

namespace countdown_timer {

class CountdownTimer : public QObject
{
  Q_OBJECT
  Q_PROPERTY(qint64 timeLeft READ timeLeft WRITE setTimeout NOTIFY timeLeftChanged)
  Q_PROPERTY(bool isActive READ isActive NOTIFY activityChanged)

public:
  explicit CountdownTimer(QObject* parent = nullptr);
  ~CountdownTimer();

  qint64 timeLeft() const { return time_left_; }
  bool isActive() const { return is_active_; }

signals:
  void activityChanged(bool is_active);
  void timeLeftChanged(qint64 time_left);
  void timeout();

public slots:
  void start();
  void stop();

  void setTimeout(int seconds);

protected:
  void timerEvent(QTimerEvent* event) override;

private:
  void setTimeLeft(qint64 time_left);

private:
  int timer_id_;
  qint64 time_left_;
  bool is_active_;
};

} // namespace countdown_timer

#endif // COUNTDOWN_TIMER_COUNTDOWN_TIMER_H
