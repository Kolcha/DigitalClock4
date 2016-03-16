#include "autostart.h"

#include <QApplication>
#include <QDir>

#ifdef Q_OS_LINUX
#include <QFile>
#include <QFileInfo>
#include <QSettings>

static QString GetAppFileName() {
  QFileInfo fi(QApplication::applicationFilePath());
  return fi.fileName();
}

static QString GetAutoStartDir() {
  return QDir::home().absoluteFilePath(".config/autostart");
}

static QString GetDesktopFile() {
  QDir auto_start_dir(GetAutoStartDir());
  return auto_start_dir.absoluteFilePath(GetAppFileName() + ".desktop");
}

static QString GetExecPath() {
  QString exec_path = QApplication::applicationFilePath() + ".sh";
  exec_path.replace(' ', "\\ ");
  return exec_path;
}

static bool IsFileCorrect(const QString& desktop_file) {
  QSettings startup_file(desktop_file, QSettings::IniFormat);
  return startup_file.value("Desktop Entry/Exec").toString() == GetExecPath();
}


bool IsAutoStartEnabled() {
  QString desktop_file = GetDesktopFile();
  return QFile::exists(desktop_file) && IsFileCorrect(desktop_file);
}

void SetAutoStart(bool enable) {
  QString desktop_file = GetDesktopFile();
  if (enable) {
    if (QFile::exists(desktop_file) && IsFileCorrect(desktop_file)) return;
    QString startup_dir = GetAutoStartDir();
    if (!QFile::exists(startup_dir)) QDir::home().mkpath(startup_dir);

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
          QApplication::applicationName(),
          QApplication::applicationName() + " " + QApplication::applicationVersion() + " by " + QApplication::organizationName(),
          GetExecPath(),
          QApplication::applicationFilePath() + ".svg");

    QFile file(desktop_file);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(desktop_data.toLocal8Bit());
    file.close();
  } else {
    QFile::remove(desktop_file);
  }
}
#endif

#ifdef Q_OS_MAC
#include <QFile>

static QString GetAutoStartDir() {
  return QDir::home().absoluteFilePath("Library/LaunchAgents");
}

static QString GetPackageName() {
  QStringList domain_parts = QApplication::organizationDomain().split('.', QString::SkipEmptyParts);
  std::reverse(domain_parts.begin(), domain_parts.end());
  return domain_parts.join('.');
}

static QString GetPlistFile() {
  QDir auto_start_dir(GetAutoStartDir());
  return auto_start_dir.absoluteFilePath(GetPackageName() + ".plist");
}


bool IsAutoStartEnabled() {
  return QFile::exists(GetPlistFile());
}

void SetAutoStart(bool enable) {
  QString plist_file = GetPlistFile();
  if (enable) {
    if (QFile::exists(plist_file)) return;
    QString startup_dir = GetAutoStartDir();
    if (!QFile::exists(startup_dir)) QDir::home().mkpath(startup_dir);

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
        .arg(GetPackageName(), QApplication::applicationFilePath());
    QFile file(plist_file);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(plist_data.toLocal8Bit());
    file.close();
  } else {
    QFile::remove(plist_file);
  }
}
#endif

#ifdef Q_OS_WIN
#include <QSettings>

bool IsAutoStartEnabled() {
  QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
  return settings.value(QApplication::applicationName()).toString() == QDir::toNativeSeparators(QApplication::applicationFilePath());
}

void SetAutoStart(bool enable) {
  QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
  if (enable) {
    settings.setValue(QApplication::applicationName(), QDir::toNativeSeparators(QApplication::applicationFilePath()));
  } else {
    settings.remove(QApplication::applicationName());
  }
}
#endif
