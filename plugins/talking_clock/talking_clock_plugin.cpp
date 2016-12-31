/*
    Digital Clock: talking clock plugin
    Copyright (C) 2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "talking_clock_plugin.h"

#include "plugin_settings.h"

#include "talking_clock_settings.h"
#include "gui/settings_dialog.h"


namespace talking_clock {

TalkingClockPlugin::TalkingClockPlugin() : started_(false)
{
  InitTranslator(QLatin1String(":/talking_clock/talking_clock_"));
  info_.display_name = tr("Talking clock");
  info_.description = tr("Announces time with selected period.");
  InitIcon(":/talking_clock/icon.svg");
}

void TalkingClockPlugin::Start()
{
  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
  settings_->TrackChanges(true);
  settings_->Load();
  started_ = true;
}

void TalkingClockPlugin::Stop()
{
  started_ = false;
}

void TalkingClockPlugin::Configure()
{
  // load current settings to dialog
  QSettings::SettingsMap curr_settings;
  InitDefaults(&curr_settings);
  if (!started_) {
    settings_->SetDefaultValues(curr_settings);
    settings_->TrackChanges(true);
  }
  for (auto iter = curr_settings.begin(); iter != curr_settings.end(); ++iter) {
    *iter = settings_->GetOption(iter.key());
  }

  SettingsDialog dlg(curr_settings);

  // connect main signals/slots
  connect(&dlg, SIGNAL(OptionChanged(QString,QVariant)),
          settings_, SLOT(SetOption(QString,QVariant)));
  connect(&dlg, SIGNAL(accepted()), settings_, SLOT(Save()));
  connect(&dlg, SIGNAL(rejected()), settings_, SLOT(Load()));

  dlg.exec();
}

void TalkingClockPlugin::TimeUpdateListener()
{
}

} // namespace talking_clock
