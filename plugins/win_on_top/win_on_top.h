/*
    Digital Clock: window on top plugin
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

#ifndef WIN_ON_TOP_H
#define WIN_ON_TOP_H

#include <Windows.h>
#include "iclock_plugin.h"

namespace win_on_top {

class WinOnTop : public IClockPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "win_on_top.json")
  Q_INTERFACES(IClockPlugin)

public:
  WinOnTop();

public slots:
  void Start();
  void Stop();

private:
  HMODULE lib_;
};

} // namespace win_on_top

#endif // WIN_ON_TOP_H
