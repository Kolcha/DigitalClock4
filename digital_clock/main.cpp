#include "main_window.h"

#include <QApplication>
#include <QIcon>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  app.setApplicationDisplayName("Digital Clock");
  app.setApplicationName("Digital Clock");
  app.setApplicationVersion("4.4.6+");
  app.setOrganizationName("Nick Korotysh");
  app.setOrganizationDomain("digitalclock4.sourceforge.net");
  app.setWindowIcon(QIcon(":/clock/images/clock.svg"));
  app.setQuitOnLastWindowClosed(false);
  app.setAttribute(Qt::AA_UseHighDpiPixmaps);

  // install app translators
  QTranslator app_translator;
  QTranslator qt_translator;
  QStringList ui_languages = QLocale::system().uiLanguages();
  foreach (QString locale, ui_languages) {
    locale = QLocale(locale).name();
    if (app_translator.load(QLatin1String(":/clock/languages/digital_clock_") + locale)) {
      if (qt_translator.load(QLatin1String("qt_") + locale,
                             QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        app.installTranslator(&app_translator);
        app.installTranslator(&qt_translator);
        break;
      }
      app_translator.load(QString()); // unload()
    } else if (locale == QLatin1String("C") /* overrideLanguage == "English" */) {
      // use built-in
      break;
    } else if (locale.startsWith(QLatin1String("en")) /* "English" is built-in */) {
      // use built-in
      break;
    } else if (locale.contains("ua", Qt::CaseInsensitive)) { /* Ukrainian, use russian */
      app_translator.load(QLatin1String(":/clock/languages/digital_clock_ru"));
      qt_translator.load(QLatin1String("qt_ru"),
                         QLibraryInfo::location(QLibraryInfo::TranslationsPath));
      app.installTranslator(&app_translator);
      app.installTranslator(&qt_translator);
      break;
    }
  }

  digital_clock::MainWindow wnd;
  wnd.show();

  return app.exec();
}
