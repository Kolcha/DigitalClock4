/*
    Digital Clock: clock common library
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

#ifndef CONFIG_SERIALIZATION_H
#define CONFIG_SERIALIZATION_H

#include <QSettings>

#include "clock_common_global.h"

namespace config {

CLOCK_COMMON_EXPORT void ExportDCS(const QString& filename, const QSettings::SettingsMap& sm);
CLOCK_COMMON_EXPORT void ImportDCS(const QString& filename, QSettings::SettingsMap& sm);

CLOCK_COMMON_EXPORT void ExportINI(const QString& filename, const QSettings::SettingsMap& sm);
CLOCK_COMMON_EXPORT void ImportINI(const QString& filename, QSettings::SettingsMap& sm);

} // namespace config

#endif // CONFIG_SERIALIZATION_H
