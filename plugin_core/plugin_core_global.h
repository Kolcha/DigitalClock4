/*
    Digital Clock: plugin core library
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

/*!
 * @file plugin_core_global.h
 *
 * @brief File wih export/import macro.
 *
 * There are some macroses to correctly export/import everything from library.
 * You don't need in including this file somewhere, it already included where it really needed.
 */

#ifndef PLUGIN_CORE_GLOBAL_H
#define PLUGIN_CORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLUGIN_CORE_LIBRARY)
#  define PLUGIN_CORE_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define PLUGIN_CORE_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PLUGIN_CORE_GLOBAL_H
