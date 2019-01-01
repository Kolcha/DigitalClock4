/*
    Digital Clock: schedule plugin
    Copyright (C) 2017-2019  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef SCHEDULE_NOTIFICATION_H
#define SCHEDULE_NOTIFICATION_H

#include <QObject>

namespace schedule {

class Notification
{
  Q_GADGET

  Q_PROPERTY(Type type READ type WRITE setType)
  Q_PROPERTY(int timeout READ timeout WRITE setTimeout)

public:
  enum Type { TrayMessage, MessageBox };
  Q_ENUM(Type)

  Notification();

  Type type() const;
  int timeout() const;
  bool playSound() const;
  QString soundFile() const;

  void setType(const Type t);
  void setTimeout(const int t);
  void setPlaySound(const bool p);
  void setSoundFile(const QString& f);

private:
  Type type_;
  int timeout_;
  bool play_sound_;
  QString sound_file_;
};

} // namespace schedule

Q_DECLARE_METATYPE(schedule::Notification)

#endif // SCHEDULE_NOTIFICATION_H
