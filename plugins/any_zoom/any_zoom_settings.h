/*
    Digital Clock: any zoom plugin
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

#ifndef ANY_ZOOM_SETTINGS_H
#define ANY_ZOOM_SETTINGS_H

#include <QSettings>

namespace any_zoom {

// any zoom plugin settings keys
const char* const OPT_CURRENT_ZOOM_X = "plugins/any_zoom/zoom_x";
const char* const OPT_CURRENT_ZOOM_Y = "plugins/any_zoom/zoom_y";

inline void InitDefaults(QSettings::SettingsMap* defaults)
{
  defaults->insert(OPT_CURRENT_ZOOM_X, 125);
  defaults->insert(OPT_CURRENT_ZOOM_Y, 125);
}

} // namespace any_zoom

#endif // ANY_ZOOM_SETTINGS_H
