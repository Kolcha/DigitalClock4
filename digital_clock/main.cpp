/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "main_window.h"

#include <QApplication>
#include <QIcon>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char* argv[])
{
  // set application info
  QApplication::setApplicationDisplayName("Digital Clock");
  QApplication::setApplicationName("Digital Clock");
  QApplication::setApplicationVersion("4.6.0+");
  QApplication::setOrganizationName("Nick Korotysh");
  QApplication::setOrganizationDomain("digitalclock4.sourceforge.net");

  // set application attributes
  QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#ifdef Q_OS_MACOS
  QApplication::setAttribute(Qt::AA_DontShowIconsInMenus);
#endif

  QApplication app(argc, argv);
  app.setWindowIcon(QIcon(":/clock/images/clock.svg.p"));
  app.setQuitOnLastWindowClosed(false);

  // install app translators
  QTranslator app_translator;
  QTranslator qt_translator;
  QStringList ui_languages = QLocale::system().uiLanguages();
  foreach (QString locale, ui_languages) {
    locale = QLocale(locale).name();
    if (locale == QLatin1String("C") ||               // overrideLanguage == "English"
        locale.startsWith(QLatin1String("en")))       // "English" is built-in
      break;                                          // use built-in

    if (locale.contains("ua", Qt::CaseInsensitive))   // Ukrainian,
      locale = QLatin1String("ru");                   // use Russian

    if (app_translator.load(QLatin1String(":/clock/languages/digital_clock_") + locale)) {
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

  digital_clock::MainWindow wnd;
  wnd.show();

  return app.exec();
}
