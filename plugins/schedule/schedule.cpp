#include <QMenu>
#include "schedule.h"

Schedule::Schedule() {
}

void Schedule::Init(QWidget* main_wnd) {
  parent_ = main_wnd;
}

void Schedule::Start() {
  tray_icon_ = new QSystemTrayIcon(QIcon(":/schedule.svg"));
  tray_menu_ = new QMenu();
  tray_menu_->addAction(QIcon(":/settings.svg"), "Settings", this, SLOT(Configure()));
  tray_icon_->setContextMenu(tray_menu_);
  tray_icon_->setToolTip("Digital Clock Schedule Plugin");
  tray_icon_->setVisible(true);
  connect(tray_icon_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          this, SLOT(TrayActivated(QSystemTrayIcon::ActivationReason)));
}

void Schedule::Stop() {
  tray_icon_->setVisible(false);
  delete tray_icon_;
  delete tray_menu_;
}

void Schedule::Configure() {
}

void Schedule::TimeUpdateListener(const QString&) {
}

void Schedule::TrayActivated(QSystemTrayIcon::ActivationReason reason) {
  if (reason == QSystemTrayIcon::DoubleClick) {
    Configure();
  }
}
