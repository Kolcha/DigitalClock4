/*
    Digital Clock: talking clock plugin
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

#include "talking_clock_plugin.h"


namespace talking_clock {

TalkingClockPlugin::TalkingClockPlugin()
{
  InitTranslator(QLatin1String(":/talking_clock/talking_clock_"));
  info_.display_name = tr("Talking clock");
  info_.description = tr("Announces time with selected period.");
  InitIcon(":/talking_clock/icon.svg");
}

void TalkingClockPlugin::Start()
{
}

void TalkingClockPlugin::Stop()
{
}

void TalkingClockPlugin::Configure()
{
}

void TalkingClockPlugin::TimeUpdateListener()
{
}

} // namespace talking_clock
