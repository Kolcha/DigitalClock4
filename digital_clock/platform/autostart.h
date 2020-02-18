/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2015-2020  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef AUTOSTART_H
#define AUTOSTART_H

/*!
 * Check autostart status.
 * @return true if enabled, false if disabled
 */
bool IsAutoStartEnabled();
/*!
 * Change autostart status (enable/disable).
 * @param enable - true to enable, false to disable
 */
void SetAutoStart(bool enable);

#endif // AUTOSTART_H
