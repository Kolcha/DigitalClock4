#include <QApplication>
#include "tray_control.h"

TrayControl::TrayControl(QObject *parent)
  : QObject(parent) {
  CreateActions();
  CreateTrayIcon();
}

void TrayControl::CreateActions() {
  settings_action_ = new QAction(tr("&Settings"), this);
  connect(settings_action_, SIGNAL(triggered()), this, SIGNAL(ShowSettingsDlg()));

  about_action_ = new QAction(tr("&About"), this);
  connect(about_action_, SIGNAL(triggered()), this, SIGNAL(ShowAboutDlg()));

  about_qt_action_ = new QAction(tr("About &Qt"), this);
  connect(about_qt_action_, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  exit_action_ = new QAction(tr("&Exit"), this);
  connect(exit_action_, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void TrayControl::CreateTrayIcon() {
  tray_menu_ = new QMenu();
  tray_menu_->addAction(settings_action_);
  tray_menu_->addSeparator();
  tray_menu_->addAction(about_action_);
  tray_menu_->addAction(about_qt_action_);
  tray_menu_->addSeparator();
  tray_menu_->addAction(exit_action_);

  tray_icon_ = new QSystemTrayIcon(this);
  tray_icon_->setContextMenu(tray_menu_);
  tray_icon_->setIcon(QIcon(":/images/clock_24.png"));
  tray_icon_->setToolTip(qApp->applicationDisplayName());
  tray_icon_->show();
}
