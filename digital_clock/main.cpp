#include "gui/main_window.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  a.setApplicationDisplayName("Digital Clock");
  a.setApplicationName("Digital Clock");
  a.setApplicationVersion("4.2.5");
  a.setOrganizationName("Nick Korotysh");
  a.setWindowIcon(QIcon(":/images/clock.svg"));
  a.setQuitOnLastWindowClosed(false);

  QTranslator qt_translator;
  qt_translator.load("qt_" + QLocale::system().name(),
                     QLibraryInfo::location(QLibraryInfo::TranslationsPath));
  a.installTranslator(&qt_translator);

  QTranslator app_translator;
  app_translator.load(":/languages/digital_clock_" + QLocale::system().name());
  a.installTranslator(&app_translator);

  digital_clock::MainWindow w;
  w.Init();
  w.show();

  return a.exec();
}
