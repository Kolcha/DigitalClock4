#include <QMenu>
#include <QApplication>
#include "tray_control.h"

namespace digital_clock {
namespace gui {

TrayControl::TrayControl(QWidget* parent) : QObject(parent) {
  QMenu* tray_menu = new QMenu(parent);
  tray_menu->addAction(QIcon(":/clock/images/settings.svg"), tr("&Settings"),
                       this, SIGNAL(ShowSettingsDlg()));
  tray_menu->addAction(QIcon(":/clock/images/about.svg"), tr("&About"),
                       this, SIGNAL(ShowAboutDlg()));
  tray_menu->addSeparator();
  tray_menu->addAction(QIcon(":/clock/images/update.svg"), tr("&Update"),
                       this, SIGNAL(CheckForUpdates()));
  tray_menu->addSeparator();
  tray_menu->addAction(QIcon(":/clock/images/quit.svg"), tr("&Quit"),
                       this, SIGNAL(AppExit()));

  tray_icon_ = new QSystemTrayIcon(this);
  tray_icon_->setContextMenu(tray_menu);
  tray_icon_->setIcon(QIcon(":/clock/images/clock.svg"));
  tray_icon_->setToolTip(qApp->applicationDisplayName() + " " + qApp->applicationVersion());
  tray_icon_->show();
  connect(tray_icon_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          this, SLOT(TrayEventHandler(QSystemTrayIcon::ActivationReason)));
}

QSystemTrayIcon* TrayControl::GetTrayIcon() {
  return tray_icon_;
}

void TrayControl::TrayEventHandler(QSystemTrayIcon::ActivationReason reason) {
  if (reason == QSystemTrayIcon::DoubleClick) emit ShowSettingsDlg();
}

} // namespace gui
} // namespace digital_clock
