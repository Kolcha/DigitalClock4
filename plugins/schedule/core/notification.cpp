/*
    Digital Clock: schedule plugin
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

#include "notification.h"

namespace schedule {

Notification::Notification() :
  type_(TrayMessage),
  timeout_(10),
  play_sound_(false)
{
}

Notification::Type Notification::type() const
{
  return type_;
}

int Notification::timeout() const
{
  return timeout_;
}

bool Notification::playSound() const
{
  return play_sound_;
}

QString Notification::soundFile() const
{
  return sound_file_;
}

void Notification::setType(const Notification::Type t)
{
  type_ = t;
}

void Notification::setTimeout(const int t)
{
  timeout_ = t;
}

void Notification::setPlaySound(const bool p)
{
  play_sound_ = p;
}

void Notification::setSoundFile(const QString& f)
{
  sound_file_ = f;
}

} // namespace schedule
