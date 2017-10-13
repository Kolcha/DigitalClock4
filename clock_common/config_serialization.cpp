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

#include "config_serialization.h"

#include <QFile>
#include <QDataStream>

namespace config {

void ExportDCS(const QString& filename, const QSettings::SettingsMap& sm)
{
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly)) return;
  QDataStream stream(&file);
  stream << sm;
  file.close();
}

void ImportDCS(const QString& filename, QSettings::SettingsMap& sm)
{
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly)) return;
  QDataStream stream(&file);
  stream >> sm;
  file.close();
}

void ExportINI(const QString& filename, const QSettings::SettingsMap& sm)
{
  QSettings s(filename, QSettings::IniFormat);
  for (auto i = sm.cbegin(); i != sm.cend(); ++i) s.setValue(i.key(), i.value());
}

void ImportINI(const QString& filename, QSettings::SettingsMap& sm)
{
  QSettings s(filename, QSettings::IniFormat);
  for (auto& key : s.allKeys()) sm[key] = s.value(key);
}

} // namespace config
