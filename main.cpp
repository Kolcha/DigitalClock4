#include "gui/main_window.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  a.setApplicationDisplayName("Digital Clock");
  a.setApplicationName("Digital Clock");
  a.setApplicationVersion("4.2.0");
  a.setOrganizationName("Nick Korotysh");
  a.setWindowIcon(QIcon(":/images/about_qt.svg"));

  QString path = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
  QTranslator qt_translator;
  qt_translator.load("qt_" + QLocale::system().name(), path);
  a.installTranslator(&qt_translator);

  QTranslator app_translator;
  app_translator.load("digital_clock_" + QLocale::system().name(), path);
  a.installTranslator(&app_translator);

  MainWindow w;
  w.Init();
  w.show();

  return a.exec();
}
