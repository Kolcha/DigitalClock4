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

#include "autostart.h"

#include <QCoreApplication>
#include <QDir>
#include <QSettings>

static const char* const REG_RUN_KEY = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";

static QString GetAppKey()
{
  QString key = QCoreApplication::applicationName();
#ifdef PORTABLE_VERSION
  QByteArray path = QCoreApplication::applicationFilePath().toUtf8();
  quint16 checksum = qChecksum(path.data(), path.size());
  key.remove(QLatin1Char(' '));
  key += QStringLiteral("-") + QString::number(checksum);
#endif
  return key;
}

bool IsAutoStartEnabled()
{
  QSettings settings(REG_RUN_KEY, QSettings::NativeFormat);
  return settings.value(GetAppKey()).toString() == QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
}

void SetAutoStart(bool enable)
{
  QSettings settings(REG_RUN_KEY, QSettings::NativeFormat);
  if (enable) {
    settings.setValue(GetAppKey(), QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
  } else {
    settings.remove(GetAppKey());
  }
}
