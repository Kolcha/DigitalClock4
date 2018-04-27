/*
    Digital Clock: timetracker plugin
    Copyright (C) 2018  Nick Korotysh <nick.korotysh@gmail.com>

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
#ifndef TIMETRACKER_TRACKER_WIDGET_H
#define TIMETRACKER_TRACKER_WIDGET_H

#include <QLabel>

namespace timetracker {

class TrackerWidget : public QLabel
{
  Q_OBJECT

public:
  explicit TrackerWidget(QWidget* parent = nullptr);

signals:
  void clicked();
  void doubleClicked();

protected:
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseDoubleClickEvent(QMouseEvent* event) override;
};

} // namespace timetracker

#endif // TIMETRACKER_TRACKER_WIDGET_H
