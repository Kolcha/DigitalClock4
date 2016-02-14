#include "tray_control.h"

#include <QMenu>
#include <QIcon>
#include <QApplication>

namespace digital_clock {
namespace gui {

TrayControl::TrayControl(QWidget* parent) : QObject(parent) {
  QMenu* tray_menu = new QMenu(parent);
  tray_menu->addAction(QIcon(":/clock/images/settings.svg").pixmap(16), tr("&Settings"),
                       this, SIGNAL(ShowSettingsDlg()));
  tray_menu->addAction(QIcon(":/clock/images/about.svg").pixmap(16), tr("&About"),
                       this, SIGNAL(ShowAboutDlg()));
  tray_menu->addSeparator();
  tray_menu->addAction(QIcon(":/clock/images/update.svg").pixmap(16), tr("&Update"),
                       this, SIGNAL(CheckForUpdates()));
  tray_menu->addSeparator();
  tray_menu->addAction(QIcon(":/clock/images/quit.svg").pixmap(16), tr("&Quit"),
                       this, SIGNAL(AppExit()));

  tray_icon_ = new QSystemTrayIcon(QApplication::windowIcon(), this);
  tray_icon_->setVisible(true);
  tray_icon_->setContextMenu(tray_menu);
  tray_icon_->setToolTip(QApplication::applicationDisplayName() + " " + QApplication::applicationVersion());
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
