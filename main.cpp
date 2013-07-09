#include "gui/main_window.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  a.setApplicationDisplayName("Digital Clock");
  a.setApplicationName("Digital Clock");
  a.setApplicationVersion("4.1.4");
  a.setOrganizationName("Nick Korotysh");
  a.setWindowIcon(QIcon(":/images/about_qt.svg"));

  MainWindow w;
  w.Init();
  w.show();

  return a.exec();
}
