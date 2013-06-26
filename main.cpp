#include "gui/main_window.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  a.setApplicationDisplayName("Digital Clock 4");
  a.setApplicationName("Digital Clock 4");
  a.setApplicationVersion("4.1.4");
  a.setOrganizationName("Nick Korotysh");

  MainWindow w;
  w.setWindowIcon(QIcon(":/images/clock_24.png"));
  w.show();

  return a.exec();
}
