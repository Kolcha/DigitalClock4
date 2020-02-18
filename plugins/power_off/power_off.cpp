/*
    Digital Clock: power off plugin
    Copyright (C) 2013-2020  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "power_off.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

#include "plugin_settings.h"
#include "message_box.h"

#include "power_off_settings.h"

#include "gui/settings_dialog.h"

namespace power_off {

PowerOff::PowerOff() : active_(false)
{
  InitTranslator(QLatin1String(":/power_off/lang/power_off_"));
  info_.display_name = tr("Auto power off");
  info_.description = tr("Shutdown system at specified time.");
  InitIcon(":/power_off/icon.svg.p");
}

void PowerOff::Start()
{
  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
  settings_->Load();

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

void PowerOff::Configure()
{
  SettingsDialog* dialog = new SettingsDialog();
  connect(dialog, &SettingsDialog::destroyed, this, &PowerOff::configured);
  // load current settings to dialog
  connect(settings_, SIGNAL(OptionChanged(QString,QVariant)),
          dialog, SLOT(SettingsListener(QString,QVariant)));
  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
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

void PowerOff::TimeUpdateListener()
{
  QString off_time = settings_->GetOption(OPT_TIME).value<QTime>().toString();
  QString curr_time = QTime::currentTime().toString();
  if (off_time != curr_time || active_) return;
  active_ = true;
  TMessageBox msg;
  msg.setIcon(QMessageBox::Warning);
  msg.setWindowTitle(tr("System shutdown"));
  msg.setText(tr("Warning! Shutdown will happen after a few seconds."));
  msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
  msg.setDefaultButton(QMessageBox::Ok);
  msg.setAutoClose(true);
  msg.setTimeout(20);
  if (msg.exec() != QMessageBox::Ok) return;
  UINT flags = EWX_SHUTDOWN;
  settings_->GetOption(OPT_FORCE).toBool() ? flags |= EWX_FORCE : flags |= EWX_FORCEIFHUNG;
  ExitWindowsEx(flags, 0);
}

} // namespace power_off
