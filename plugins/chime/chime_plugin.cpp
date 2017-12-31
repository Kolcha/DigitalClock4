/*
    Digital Clock: chime plugin
    Copyright (C) 2016-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTime>

#include "plugin_settings.h"

#include "core/chime_settings.h"
#include "core/repeat_type.h"

#include "gui/settings_dialog.h"


namespace chime {

static bool is_quiet_time(const QTime& c_time, const QTime& s_time, const QTime& e_time)
{
  if (s_time <= e_time) return (s_time <= c_time) && (c_time <= e_time);
  return ((s_time <= c_time) && (c_time <= QTime(23, 59, 59, 999))) || ((QTime(0, 0) <= c_time) && (c_time <= e_time));
}


ChimePlugin::ChimePlugin() : started_(false), playback_allowed_(true), player_(nullptr)
{
  info_.display_name = tr("Chime");
  info_.description = tr("Chimes with selected period."
                         "This plugin replaces all chiming plugins:\n"
                         "\"Every hour signal\", \"Quarter of an hour signal\", \"Tower clock hour signal\".\n"
                         "Sounds from these plugins are included as default sounds.");
  InitIcon(":/chime/icon.svg.p");
}

void ChimePlugin::Start()
{
  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
  settings_->TrackChanges(true);
  settings_->Load();
  player_ = new QMediaPlayer(this);
  player_->setPlaylist(new QMediaPlaylist(player_));
  started_ = true;
}

void ChimePlugin::Stop()
{
  started_ = false;
  player_->stop();
  player_->playlist()->clear();
}

void ChimePlugin::Configure()
{
  SettingsDialog* dialog = new SettingsDialog();
  connect(dialog, &SettingsDialog::destroyed, this, &ChimePlugin::configured);
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
  if (!started_) return;

  QTime cur_time = QTime::currentTime();

  if (cur_time.minute() == 0 && !isQuietTime(cur_time)) {   // hour
    if (playback_allowed_ && settings_->GetOption(OPT_EVERY_HOUR_ENABLED).toBool()) {
      player_->playlist()->clear();
      int count = 1;
      if (static_cast<Repeat>(settings_->GetOption(OPT_EVERY_HOUR_REPEAT).toInt()) == Repeat::Dynamic) {
        count = cur_time.hour();
        if (count == 0) count = 12;
        if (count > 12) count -= 12;
      }
      Q_ASSERT(count > 0);
      for (int i = 0; i < count; ++i)
        player_->playlist()->addMedia(settings_->GetOption(OPT_EVERY_HOUR_SIGNAL).toUrl());
      player_->setVolume(settings_->GetOption(OPT_EVERY_HOUR_VOLUME).toInt());
      player_->play();
      playback_allowed_ = false;
    }
  }

  if (cur_time.minute() % 15 == 0 && cur_time.minute() != 0 && !isQuietTime(cur_time)) {    // quarter
    if (playback_allowed_ && settings_->GetOption(OPT_QUARTER_HOUR_ENABLED).toBool()) {
      player_->playlist()->clear();
      int count = 1;
      if (static_cast<Repeat>(settings_->GetOption(OPT_QUARTER_HOUR_REPEAT).toInt()) == Repeat::Dynamic) {
        count = cur_time.minute() / 15;
      }
      Q_ASSERT(count > 0);
      for (int i = 0; i < count; ++i)
        player_->playlist()->addMedia(settings_->GetOption(OPT_QUARTER_HOUR_SIGNAL).toUrl());
      player_->setVolume(settings_->GetOption(OPT_QUARTER_HOUR_VOLUME).toInt());
      player_->play();
      playback_allowed_ = false;
    }
  }

  playback_allowed_ = (cur_time.minute() % 15 != 0);
}

bool ChimePlugin::isQuietTime(const QTime& cur_time)
{
  if (settings_->GetOption(OPT_QUIET_HOURS_ENABLED).toBool()) {
    QTime qs_time = settings_->GetOption(OPT_QUIET_HOURS_START).toTime();
    QTime qe_time = settings_->GetOption(OPT_QUIET_HOURS_END).toTime();
    return is_quiet_time(cur_time, qs_time, qe_time);
  }
  return false;
}

} // namespace chime
