/*
    Digital Clock: tower clock signal plugin
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

#ifndef TOWER_CLOCK_SIGNAL_H
#define TOWER_CLOCK_SIGNAL_H

#include "iclock_plugin.h"


class QSound;

namespace tower_clock_signal {

class TowerCLockSignal : public IClockPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "tower_clock_signal.json")
  Q_INTERFACES(IClockPlugin)

public:
  TowerCLockSignal();

public slots:
  void Start();
  void Stop();

  void TimeUpdateListener();

private:
  QSound* bell_sound_;
};

} // namespace tower_clock_signal

#endif // TOWER_CLOCK_SIGNAL_H
