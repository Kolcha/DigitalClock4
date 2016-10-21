/*
    Digital Clock: chime plugin
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

#include "chime_plugin.h"

#include "plugin_settings.h"

#include "core/chime_settings.h"

#include "gui/settings_dialog.h"


namespace chime {

ChimePlugin::ChimePlugin() : started_(false)
{
  InitTranslator(QLatin1String(":/chime/chime_"));
  info_.display_name = tr("Chime");
  info_.description = tr("Chimes with selected period."
                         "This plugin replaces all chiming plugins:\n"
                         "\"Every hour signal\", \"Quarter of an hour signal\", \"Tower clock hour signal\".\n"
                         "Sounds from these plugins are included as default sounds.");
  InitIcon(":/chime/icon.svg");

  player_ = new QMediaPlayer(this);
}

void ChimePlugin::Start()
{
  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
  settings_->TrackChanges(true);
  connect(settings_, &PluginSettings::OptionChanged, this, &ChimePlugin::SettingsUpdateListener);
  settings_->Load();
  started_ = true;
}

void ChimePlugin::Stop()
{
  started_ = false;
  player_->stop();
}

void ChimePlugin::Configure()
{
  SettingsDialog* dialog = new SettingsDialog();
  // load current settings to dialog
  QSettings::SettingsMap curr_settings;
  InitDefaults(&curr_settings);
  if (!started_) {
    settings_->SetDefaultValues(curr_settings);
    connect(settings_, SIGNAL(OptionChanged(QString,QVariant)),
            this, SLOT(SettingsUpdateListener(QString,QVariant)));
    settings_->TrackChanges(true);
  }
  for (auto iter = curr_settings.begin(); iter != curr_settings.end(); ++iter) {
    *iter = settings_->GetOption(iter.key());
  }
  dialog->Init(curr_settings);
  // connect main signals/slots
  connect(dialog, SIGNAL(OptionChanged(QString,QVariant)),
          settings_, SLOT(SetOption(QString,QVariant)));
  connect(dialog, SIGNAL(accepted()), settings_, SLOT(Save()));
  connect(dialog, SIGNAL(rejected()), settings_, SLOT(Load()));
  dialog->show();
}

void ChimePlugin::TimeUpdateListener()
{
  //
}

void ChimePlugin::SettingsUpdateListener(const QString& key, const QVariant& value)
{
  //
}

} // namespace chime
