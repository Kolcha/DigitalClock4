/*
    Digital Clock: random position plugin
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

#ifndef RANDOM_POSITION_RANDOM_POSITION_H
#define RANDOM_POSITION_RANDOM_POSITION_H

#include "iclock_plugin.h"
#include "iplugin_init.h"

namespace random_position {

class RandomPosition : public IClockPlugin, public IWidgetPluginInit
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "random_position.json")
  Q_INTERFACES(IClockPlugin IWidgetPluginInit)

public:
  RandomPosition();

  void Init(QWidget* main_wnd) override;

public slots:
  void Start() override;
  void Stop() override;

  void TimeUpdateListener() override;

private:
  QHash<QWidget*, QPoint> windows_;
  bool is_active_;
  int interval_counter_;
};

} // namespace random_position

#endif // RANDOM_POSITION_RANDOM_POSITION_H
