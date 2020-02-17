/*
    Digital Clock - beautiful customizable clock with plugins
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

#ifdef PORTABLE_VERSION
#include <QDir>
#include <QFileInfo>
#endif
#ifdef HAVE_SINGLEAPPLICATION
#include <singleapplication.h>
#else
#include <QApplication>
#endif
#include <QIcon>
#include <QTranslator>
#include <QLibraryInfo>
#include <QPalette>

#include "settings_keys.h"
#include "settings_storage.h"
#include "core/clock_settings.h"
#include "core/clock_application.h"

int main(int argc, char* argv[])
{
  // set application info
  QApplication::setApplicationDisplayName("Digital Clock");
  QApplication::setApplicationName("Digital Clock");
  QApplication::setApplicationVersion("4.7.6+");
  QApplication::setOrganizationName("Nick Korotysh");
  QApplication::setOrganizationDomain("digitalclock4.sourceforge.net");

  // set application attributes
  QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#ifdef Q_OS_MACOS
  QApplication::setAttribute(Qt::AA_DontShowIconsInMenus);
#endif

#ifdef PORTABLE_VERSION
  QFileInfo fi{QDir::fromNativeSeparators(QLatin1String(argv[0]))};
  SettingsStorage config_backend{fi.absoluteDir().absoluteFilePath("settings.ini")};
#else
  SettingsStorage config_backend;
#endif
  digital_clock::core::ClockSettings app_config(&config_backend);
#ifdef HAVE_SINGLEAPPLICATION
  SingleApplication app(argc, argv, !app_config.GetValue(OPT_ONLY_ONE_INSTANCE).toBool());
#else
  QApplication app(argc, argv);
#endif
  app.setWindowIcon(QIcon(":/clock/images/clock.svg.p"));
  app.setQuitOnLastWindowClosed(false);

  // install app translators
  QTranslator app_translator;
  QTranslator qt_translator;
  for (QString locale : QLocale::system().uiLanguages()) {
    locale = QLocale(locale).name();
    if (locale == QLatin1String("C") ||               // overrideLanguage == "English"
        locale.startsWith(QLatin1String("en")))       // "English" is built-in
      break;                                          // use built-in

    if (locale.contains("ua", Qt::CaseInsensitive))   // Ukrainian,
      locale = QLatin1String("ru");                   // use Russian

    if (app_translator.load(QLatin1String(":/digital_clock/lang/digital_clock_") + locale)) {
      app.installTranslator(&app_translator);
      if (qt_translator.load(QLatin1String("qt_") + locale,
                             QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        app.installTranslator(&qt_translator);
      break;
    }
  }

  // small trick to get links to look better
  QPalette p;
  p.setColor(QPalette::Link, p.color(QPalette::Active, QPalette::Text));
  p.setColor(QPalette::LinkVisited, p.color(QPalette::Inactive, QPalette::Text));
  QApplication::setPalette(p);

  digital_clock::core::ClockApplication clock_app(&app_config);
  Q_UNUSED(clock_app);

  return app.exec();
}
