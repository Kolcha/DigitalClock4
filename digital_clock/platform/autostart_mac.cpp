/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2015-2019  Nick Korotysh <nick.korotysh@gmail.com>

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
#include <QFile>

static QString GetAutoStartDir()
{
  return QDir::home().absoluteFilePath("Library/LaunchAgents");
}

static QString GetPackageName()
{
  QStringList domain_parts = QCoreApplication::organizationDomain().split('.', QString::SkipEmptyParts);
  std::reverse(domain_parts.begin(), domain_parts.end());
  return domain_parts.join('.');
}

static QString GetPlistFile()
{
  QDir auto_start_dir(GetAutoStartDir());
  return auto_start_dir.absoluteFilePath(GetPackageName() + ".plist");
}


bool IsAutoStartEnabled()
{
  return QFile::exists(GetPlistFile());
}

void SetAutoStart(bool enable)
{
  QString plist_file = GetPlistFile();
  if (enable) {
    if (QFile::exists(plist_file)) return;
    QString startup_dir = GetAutoStartDir();
    if (!QFile::exists(startup_dir)) QDir::home().mkpath(startup_dir);
    // *INDENT-OFF*
    QString plist_data = QString(
          "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
          "<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
          "<plist version=\"1.0\">\n"
          "<dict>\n"
          "    <key>Label</key>\n"
          "    <string>%1</string>\n"
          "    <key>Program</key>\n"
          "    <string>%2</string>\n"
          "    <key>RunAtLoad</key>\n"
          "    <true/>\n"
          "</dict>\n"
          "</plist>\n")
        .arg(GetPackageName(), QCoreApplication::applicationFilePath());
    // *INDENT-ON*
    QFile file(plist_file);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(plist_data.toLocal8Bit());
    file.close();
  } else {
    QFile::remove(plist_file);
  }
}
