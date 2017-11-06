/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2015-2017  Nick Korotysh <nick.korotysh@gmail.com>

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
#include <QFileInfo>

static QString GetAppFileName()
{
  QFileInfo fi(QCoreApplication::applicationFilePath());
  return fi.fileName();
}

static QString GetAutoStartDir()
{
  return QDir::home().absoluteFilePath(".config/autostart");
}

static QString GetDesktopFile()
{
  QDir auto_start_dir(GetAutoStartDir());
  return auto_start_dir.absoluteFilePath(GetAppFileName() + ".desktop");
}


bool IsAutoStartEnabled()
{
  return QFile::exists(GetDesktopFile());
}

void SetAutoStart(bool enable)
{
  QString desktop_file = GetDesktopFile();
  if (enable) {
    if (QFile::exists(desktop_file)) return;
    QString startup_dir = GetAutoStartDir();
    if (!QFile::exists(startup_dir)) QDir::home().mkpath(startup_dir);

    // *INDENT-OFF*
    QString desktop_data = QString(
          "[Desktop Entry]\n"
          "Name=%1\n"
          "Comment=%2\n"
          "Type=Application\n"
          "Terminal=false\n"
          "Exec=\"%3\" --autostart\n"
          "Icon=%4\n"
          "StartupNotify=false\n")
        .arg(
          QCoreApplication::applicationName(),
          QCoreApplication::applicationName() + " by " + QCoreApplication::organizationName(),
          QCoreApplication::applicationFilePath() + ".sh",
          QCoreApplication::applicationFilePath() + ".svg");
    // *INDENT-ON*
    QFile file(desktop_file);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(desktop_data.toLocal8Bit());
    file.close();
  } else {
    QFile::remove(desktop_file);
  }
}
