#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setApplicationDisplayName("Digital Clock 4");
    a.setApplicationName("Digital Clock 4");
    a.setApplicationVersion("4.1.4");
    a.setOrganizationName("Nick Korotysh");
    Widget w;
    w.show();

    return a.exec();
}
