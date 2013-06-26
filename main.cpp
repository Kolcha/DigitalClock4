#include "core/skin_manager.h"
#include "skin/skin_drawer.h"
#include "gui/digital_clock.h"
#include <QApplication>
#include <QImage>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setApplicationDisplayName("Digital Clock 4");
    a.setApplicationName("Digital Clock 4");
    a.setApplicationVersion("4.1.4");
    a.setOrganizationName("Nick Korotysh");

    SkinManager sm;
    sm.AddSkinDir(QDir(QDir::currentPath() + "/skins"));
    sm.ListSkins();
    SkinDrawer sd;
    QObject::connect(&sm, SIGNAL(SkinFound(QDir)), &sd, SLOT(LoadSkin(QDir)));
    sm.FindSkin("Comic Sans");
    sd.SetColor(Qt::blue);
    sd.SetZoom(1.25);
    DigitalClock w;
    QObject::connect(&sd, SIGNAL(DrawingFinished(QImage)), &w, SLOT(DrawImage(QImage)));
    QObject::connect(&w, SIGNAL(ImageNeeded(QString)), &sd, SLOT(SetString(QString)));
    sd.SetString("88:88");
    w.show();

    return a.exec();
}
