/*
    Digital Clock: move clock plugin
    Copyright (C) 2014-2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef MOVE_CLOCK_MOVE_CLOCK_H
#define MOVE_CLOCK_MOVE_CLOCK_H

#include <QDesktopWidget>
#include <QTimer>
#include "iclock_plugin.h"
#include "iplugin_init.h"

namespace move_clock {

class MoveClock : public IClockPlugin, public IWidgetPluginInit
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "move_clock.json")
  Q_INTERFACES(IClockPlugin IWidgetPluginInit)

public:
  MoveClock();
  void Init(QWidget* main_wnd);

public slots:
  void Start();
  void Stop();
  void Configure();

private slots:
  void SettingsListener(const QString& key, const QVariant& value);
  void TimeoutHandler();

private:
  QWidget* clock_wnd_;
  QDesktopWidget desktop_;
  QPoint old_pos_;
  QTimer timer_;
  int x0_, ax_;
  double kx_, nx_;
  int y0_, ay_;
  double ky_, ny_;
  double t_;
  int dx_, dy_;
};

} // namespace move_clock

#endif // MOVE_CLOCK_MOVE_CLOCK_H
