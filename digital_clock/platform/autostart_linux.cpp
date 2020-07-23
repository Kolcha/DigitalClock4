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
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>

static QString GetInstanceSuffix()
{
#ifdef PORTABLE_VERSION
  QByteArray path = QCoreApplication::applicationFilePath().toUtf8();
  quint16 checksum = qChecksum(path.data(), path.size());
  return QStringLiteral("-") + QString::number(checksum);
#else
  return QString();
}

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
  return auto_start_dir.absoluteFilePath(GetAppFileName() + GetInstanceSuffix() + ".desktop");
}

static QString GetAutoStartCmd()
{
  return QString("\"%1\" --autostart").arg(QCoreApplication::applicationFilePath() + ".sh");
}

static bool IsAutoStartFileHasCorrectCmd(const QString& path, const QString& cmd)
{
  QFile f(path);

  if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QString data(f.readAll());
    f.close();

    QRegularExpression re(QLatin1String("^Exec=(.*)$"), QRegularExpression::MultilineOption);
    QRegularExpressionMatch m = re.match(data);
    if (m.hasMatch())
      return m.captured(1) == cmd;
  }

  return false;
}


bool IsAutoStartEnabled()
{
  return IsAutoStartFileHasCorrectCmd(GetDesktopFile(), GetAutoStartCmd());
}

void SetAutoStart(bool enable)
{
  QString desktop_file = GetDesktopFile();
  if (enable) {
    QString autostart_cmd = GetAutoStartCmd();
    if (IsAutoStartFileHasCorrectCmd(desktop_file, autostart_cmd)) return;
    QString startup_dir = GetAutoStartDir();
    if (!QFile::exists(startup_dir)) QDir::home().mkpath(startup_dir);

    // *INDENT-OFF*
    QString desktop_data = QString(
          "[Desktop Entry]\n"
          "Name=%1\n"
          "Comment=%2\n"
          "Type=Application\n"
          "Terminal=false\n"
          "Exec=%3\n"
          "Icon=%4\n"
          "StartupNotify=false\n")
        .arg(
          QCoreApplication::applicationName(),
          QCoreApplication::applicationName() + " by " + QCoreApplication::organizationName(),
          autostart_cmd,
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
