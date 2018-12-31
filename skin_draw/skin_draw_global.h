/*
    Digital Clock: skin draw engine
    Copyright (C) 2013-2019  Nick Korotysh <nick.korotysh@gmail.com>

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

/*!
 * @file skin_draw_global.h
 *
 * @brief File wih export/import macro.
 *
 * There some macroses to correctly export/import everything from library.
 * You are not needed in including this file somewhere, it already included where it really needed.
 */

#ifndef SKIN_DRAW_GLOBAL_H
#define SKIN_DRAW_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SKIN_DRAW_LIBRARY)
#  define SKIN_DRAWSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SKIN_DRAWSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SKIN_DRAW_GLOBAL_H
