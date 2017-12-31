/*
    Digital Clock: chime plugin
    Copyright (C) 2016-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef CHIME_CHIME_PLUGIN_H
#define CHIME_CHIME_PLUGIN_H

#include "iclock_plugin.h"


class QMediaPlayer;

namespace chime {

class ChimePlugin : public IClockPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "chime.json")
  Q_INTERFACES(IClockPlugin)

public:
  ChimePlugin();

public slots:
  void Start() override;
  void Stop() override;

  void Configure() override;

  void TimeUpdateListener() override;

private:
  bool isQuietTime(const QTime& cur_time);

  bool started_;
  bool playback_allowed_;
  QMediaPlayer* player_;
};

} // namespace chime

#endif // CHIME_CHIME_PLUGIN_H
