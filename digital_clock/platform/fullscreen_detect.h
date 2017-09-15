/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef DIGITAL_CLOCK_FULLSCREEN_DETECT_H
#define DIGITAL_CLOCK_FULLSCREEN_DETECT_H

#include <QWidget>
#include <QStringList>

namespace digital_clock {

/*!
 * Finds fullscreen windows on the same monitor as @a wid.
 * @param ignore_list - list of fullscreen windows to ignore
 * @return true if fullscreen window was found
 */
bool IsFullscreenWndOnSameMonitor(WId wid, const QStringList& ignore_list);

} // namespace digital_clock

#endif // DIGITAL_CLOCK_FULLSCREEN_DETECT_H
