#include "gui/main_window.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  a.setApplicationDisplayName("Digital Clock");
  a.setApplicationName("Digital Clock");
  a.setApplicationVersion("4.2.6");
  a.setOrganizationName("Nick Korotysh");
  a.setWindowIcon(QIcon(":/clock/images/clock.svg"));
  a.setQuitOnLastWindowClosed(false);

  QTranslator app_translator;
  QTranslator qt_translator;
  QStringList ui_languages = QLocale::system().uiLanguages();
  foreach (QString locale, ui_languages) {
    locale = QLocale(locale).name();
    if (app_translator.load(QLatin1String(":/clock/languages/digital_clock_") + locale)) {
      if (qt_translator.load(QLatin1String("qt_") + locale,
                             QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        a.installTranslator(&app_translator);
        a.installTranslator(&qt_translator);
        break;
      }
      app_translator.load(QString()); // unload()
    } else if (locale == QLatin1String("C") /* overrideLanguage == "English" */) {
      // use built-in
      break;
    } else if (locale.startsWith(QLatin1String("en")) /* "English" is built-in */) {
      // use built-in
      break;
    }
  }

  digital_clock::MainWindow w;
  w.Init();
  w.show();

  return a.exec();
}
