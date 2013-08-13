#include <QSystemTrayIcon>
#include "../gui/settings_dlg.h"
#include "manager.h"

void Manager::Init(QSystemTrayIcon* tray_icon) {
  tray_icon_ = tray_icon;
  old_icon_ = tray_icon->icon();
}

void Manager::Configure() {
  SettingsDlg* dlg = new SettingsDlg();
  dlg->show();
}

void Manager::Start() {
  tray_icon_->setIcon(QIcon(":/alarm_clock.svg"));
  // add new menu items
}

void Manager::Stop() {
  tray_icon_->setIcon(old_icon_);
  // delete added menu items
}

void Manager::GetInfo(TPluginInfo* info) {
  info->insert(PI_NAME, "Alarm / Timer");
  info->insert(PI_TYPE, "tray");
  info->insert(PI_VERSION, "2.0");
  info->insert(PI_AUTHOR, "Nick Korotysh");
  info->insert(PI_EMAIL, "nick.korotysh@gmail.com");
  info->insert(PI_COMMENT, "Set alarm or/and timer.");
  info->insert(PI_CONFIG, "true");
}

void Manager::TimeUpdateListener(const QString&) {
}
