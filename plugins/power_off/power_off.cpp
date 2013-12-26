#include <windows.h>
#include <QTranslator>
#include <QLocale>
#include <QApplication>
#include "plugin_settings.h"
#include "power_off_settings.h"
#include "gui/settings_dialog.h"
#include "power_off.h"

namespace power_off {

PowerOff::PowerOff() : active_(false) {
  translator_ = new QTranslator(this);
  translator_->load(":/power_off/power_off_" + QLocale::system().name());
  QApplication::installTranslator(translator_);
  settings_ = new PluginSettings("Nick Korotysh", "Digital Clock", this);

  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
  settings_->Load();
}

PowerOff::~PowerOff() {
  QApplication::removeTranslator(translator_);
}

void PowerOff::Start() {
  HANDLE hToken;
  TOKEN_PRIVILEGES* NewState;
  OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);
  NewState = (TOKEN_PRIVILEGES*)malloc(sizeof(TOKEN_PRIVILEGES) + sizeof(LUID_AND_ATTRIBUTES));
  NewState->PrivilegeCount = 1;
  LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &NewState->Privileges[0].Luid);
  NewState->Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
  AdjustTokenPrivileges(hToken, FALSE, NewState, 0, NULL, NULL);
  free(NewState);
  CloseHandle(hToken);
}

void PowerOff::Configure() {
  SettingsDialog* dialog = new SettingsDialog();
  // load current settings to dialog
  connect(settings_, SIGNAL(OptionChanged(QString,QVariant)),
          dialog, SLOT(SettingsListener(QString,QVariant)));
  settings_->TrackChanges(true);
  settings_->Load();
  settings_->TrackChanges(false);
  // connect main signals/slots
  connect(dialog, SIGNAL(OptionChanged(QString,QVariant)),
          settings_, SLOT(SetOption(QString,QVariant)));
  connect(dialog, SIGNAL(accepted()), settings_, SLOT(Save()));
  connect(dialog, SIGNAL(rejected()), settings_, SLOT(Load()));
  dialog->show();
}

void PowerOff::TimeUpdateListener() {
  QString off_time = settings_->GetOption(OPT_TIME).value<QTime>().toString();
  QString curr_time = QTime::currentTime().toString();
  if (off_time != curr_time || active_) return;
  active_ = true;
  ExitWindowsEx(EWX_SHUTDOWN, 0);
}

} // namespace power_off
