/*
    Digital Clock: tray color plugin
    Copyright (C) 2013-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef TRAY_COLOR_TRAY_COLOR_H
#define TRAY_COLOR_TRAY_COLOR_H

#include "iclock_plugin.h"
#include "iplugin_init.h"

#include <QIcon>

namespace tray_color {

class TrayColor : public IClockPlugin, public ITrayPluginInit
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "tray_color.json")
  Q_INTERFACES(IClockPlugin ITrayPluginInit)

public:
  TrayColor();
  void Init(QSystemTrayIcon* tray_icon);

public slots:
  void Start();
  void Stop();
  void Configure();

private:
  void RedrawTrayIcon(const QColor& color);

  QSystemTrayIcon* tray_icon_;
  QIcon old_icon_;

  bool is_enabled_;
};

} // namespace tray_color

#endif // TRAY_COLOR_TRAY_COLOR_H
