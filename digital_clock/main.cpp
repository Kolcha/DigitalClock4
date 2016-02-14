#include "main_window.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  app.setApplicationDisplayName("Digital Clock");
  app.setApplicationName("Digital Clock");
  app.setApplicationVersion("4.4.2+");
  app.setOrganizationName("Nick Korotysh");
  app.setOrganizationDomain("digitalclock4.sourceforge.net");
  app.setWindowIcon(QIcon(":/clock/images/clock.svg"));
  app.setQuitOnLastWindowClosed(false);
  app.setAttribute(Qt::AA_UseHighDpiPixmaps);

  digital_clock::MainWindow wnd;
  wnd.show();

  return app.exec();
}
