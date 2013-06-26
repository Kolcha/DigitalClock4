#include "gui/digital_clock.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setApplicationDisplayName("Digital Clock 4");
    a.setApplicationName("Digital Clock 4");
    a.setApplicationVersion("4.1.4");
    a.setOrganizationName("Nick Korotysh");
    DigitalClock w;
    w.show();

    return a.exec();
}
