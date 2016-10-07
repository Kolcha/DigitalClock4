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

#include "signal_item.h"

namespace custom_signal {

SignalItem::SignalItem(QObject* parent) : QObject(parent)
{
  period_ = 1;
  offset_ = 0;
  next_shot_ = next_time(QTime::currentTime(), period_, offset_);
  enabled_ = false;
}

int SignalItem::period() const
{
  return period_;
}

int SignalItem::offset() const
{
  return offset_;
}

int SignalItem::volume() const
{
  return volume_;
}

QUrl SignalItem::sound() const
{
  return sound_;
}

QTime SignalItem::next() const
{
  return next_shot_;
}

bool SignalItem::enabled() const
{
  return enabled_;
}

void SignalItem::setPeriod(int period)
{
  period_ = period;
  next_shot_ = next_time(QTime::currentTime(), period, offset_);
  emit nextChanged(next_shot_);
}

void SignalItem::setOffset(int offset)
{
  offset_ = offset;
  next_shot_ = next_time(QTime::currentTime(), period_, offset);
  emit nextChanged(next_shot_);
}

void SignalItem::setVolume(int volume)
{
  volume_ = volume;
}

void SignalItem::setSound(const QUrl& sound)
{
  sound_ = sound;
}

void SignalItem::setEnabled(bool enabled)
{
  enabled_ = enabled;
}

void SignalItem::checkTime(const QTime& t)
{
  if (QTime(t.hour(), t.minute()) != next_shot_) return;
  next_shot_ = next_shot_.addSecs(60*period_);
  emit nextChanged(next_shot_);
  if (enabled_) emit timeout();
}

QTime next_time(const QTime& now, const int period, const int offset)
{
  int target = (now.minute() / period + 1) * period + offset;
  if (target >= 60) {
    return QTime(now.hour(), 0).addSecs(3600);
  } else {
    return QTime(now.hour(), target);
  }
}

} // namespace custom_signal
