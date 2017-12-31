/*
    Digital Clock: countdown timer plugin
    Copyright (C) 2017-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef COUNTDOWN_TIMER_UTILITIES_H
#define COUNTDOWN_TIMER_UTILITIES_H

#include <QString>

namespace countdown_timer {

QString format_time(qint64 time_left);

} // namespace countdown_timer

#endif // COUNTDOWN_TIMER_UTILITIES_H
