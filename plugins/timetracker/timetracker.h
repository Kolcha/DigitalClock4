/*
    Digital Clock: timetracker plugin
    Copyright (C) 2018-2020  Nick Korotysh <nick.korotysh@gmail.com>

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
#ifndef TIMETRACKER_TIMETRACKER_H
#define TIMETRACKER_TIMETRACKER_H

#include <QObject>
#include <QElapsedTimer>

namespace timetracker {

class Timetracker : public QObject
{
  Q_OBJECT

public:
  explicit Timetracker(QObject* parent = nullptr);

  int elapsed() const;
  bool isActive() const;

public slots:
  void start();
  void stop();
  void reset();

private:
  QElapsedTimer timer_;
  int last_elapsed_ = 0;
};

} // namespace timetracker

#endif // TIMETRACKER_TIMETRACKER_H
