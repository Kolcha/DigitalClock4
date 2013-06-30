#include "gui/main_window.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  a.setApplicationDisplayName("Digital Clock");
  a.setApplicationName("Digital Clock 4");
  a.setApplicationVersion("4.1.4");
  a.setOrganizationName("Nick Korotysh");
  a.setWindowIcon(QIcon(":/images/clock_24.png"));

  MainWindow w;
  w.show();

  return a.exec();
}
