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

#ifndef TRAY_COLOR_SETTINGS_H
#define TRAY_COLOR_SETTINGS_H

#include <QSettings>
#include <QColor>

namespace tray_color {

// alarm plugin settings keys
const char* const OPT_TRAY_COLOR = "plugins/tray_color/color";

inline void InitDefaults(QSettings::SettingsMap* defaults)
{
  defaults->insert(OPT_TRAY_COLOR, QColor(0, 85, 255));
}

} // namespace tray_color

#endif // TRAY_COLOR_SETTINGS_H
