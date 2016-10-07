/*
    Digital Clock: spectrum clock plugin
    Copyright (C) 2013-2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef SPECTRUM_CLOCK_H
#define SPECTRUM_CLOCK_H

#include <QColor>
#include "iclock_plugin.h"
#include "iplugin_init.h"

namespace spectrum_clock {

class SpectrumClock : public ISettingsPlugin, public ISettingsPluginInit
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "spectrum_clock.json")
  Q_INTERFACES(IClockPlugin ISettingsPlugin ISettingsPluginInit)

public:
  SpectrumClock();
  void Init(const QMap<Option, QVariant>& current_settings);

public slots:
  void Start();
  void Stop();
  void TimeUpdateListener();

private:
  QColor old_color_;
  QColor old_colorize_color_;
  QColor cur_color_;
};

} // namespace spectrum_clock

#endif // SPECTRUM_CLOCK_H
