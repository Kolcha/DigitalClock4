/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2020  Nick Korotysh <nick.korotysh@gmail.com>

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
#ifndef MOUSE_TRACKER_H
#define MOUSE_TRACKER_H

#include <QObject>

#include <QPoint>

namespace digital_clock {

class MouseTrackerPrivate;

class MouseTracker : public QObject
{
  Q_OBJECT

public:
  explicit MouseTracker(QObject* parent = nullptr);
  ~MouseTracker();

signals:
  void mousePositionChanged(const QPoint& p, Qt::KeyboardModifiers m);

public slots:
  void start();
  void stop();

private:
  MouseTrackerPrivate* impl_;
};

} // namespace digital_clock

#endif // MOUSE_TRACKER_H
