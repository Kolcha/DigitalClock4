#include <QSystemTrayIcon>
#include "gui/settings_dlg.h"
#include "alarm.h"

void Alarm::Init(QSystemTrayIcon* tray_icon) {
  tray_icon_ = tray_icon;
  old_icon_ = tray_icon->icon();
}

void Alarm::Configure() {
  SettingsDlg* dlg = new SettingsDlg();
  dlg->show();
}

void Alarm::Start() {
  tray_icon_->setIcon(QIcon(":/alarm_clock.svg"));
  // add new menu items
}

void Alarm::Stop() {
  tray_icon_->setIcon(old_icon_);
  // delete added menu items
}

void Alarm::GetInfo(TPluginInfo* info) {
  info->insert(PI_NAME, "Alarm");
  info->insert(PI_TYPE, "tray");
  info->insert(PI_VERSION, "2.0");
  info->insert(PI_AUTHOR, "Nick Korotysh");
  info->insert(PI_EMAIL, "nick.korotysh@gmail.com");
  info->insert(PI_COMMENT, "Set alarm.");
  info->insert(PI_CONFIG, "true");
}

void Alarm::TimeUpdateListener(const QString&) {
}
