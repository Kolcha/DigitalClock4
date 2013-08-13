#include <QSystemTrayIcon>
#include <QMenu>
#include "../gui/settings_dlg.h"
#include "manager.h"

void Manager::Init(QSystemTrayIcon* tray_icon, QMenu* tray_menu) {
}

void Manager::Configure() {
  SettingsDlg* dlg = new SettingsDlg();
  dlg->show();
}

void Manager::Start() {
}

void Manager::Stop() {
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
