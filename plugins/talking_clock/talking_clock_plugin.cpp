/*
    Digital Clock: talking clock plugin
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

#include "talking_clock_plugin.h"

#include "plugin_settings.h"

#include <QTextToSpeech>
#include <QDateTime>

#include "talking_clock_settings.h"
#include "gui/settings_dialog.h"


namespace talking_clock {

TalkingClockPlugin::TalkingClockPlugin() :
  started_(false), speech_(nullptr), playback_allowed_(true), local_time_(true)
{
  InitTranslator(QLatin1String(":/talking_clock/talking_clock_"));
  info_.display_name = tr("Talking clock");
  info_.description = tr("Announces time with selected period.");
  InitIcon(":/talking_clock/icon.svg.p");
  time_zone_ = QTimeZone::systemTimeZone();
}

void TalkingClockPlugin::Init(const QMap<Option, QVariant>& current_settings)
{
  local_time_ = current_settings.value(OPT_DISPLAY_LOCAL_TIME, local_time_).toBool();
  QVariant tz_value = current_settings.value(OPT_TIME_ZONE, time_zone_.id());
  QByteArray tz_ba = tz_value.canConvert<QString>() ? tz_value.toString().toLatin1() : tz_value.toByteArray();
  time_zone_ = QTimeZone(tz_ba);
}

void TalkingClockPlugin::Start()
{
  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
  settings_->Load();
  started_ = true;
  InitSpeechEngine();
}

void TalkingClockPlugin::Stop()
{
  started_ = false;
  if (speech_) speech_->stop();
}

void TalkingClockPlugin::Configure()
{
  // load current settings to dialog
  QSettings::SettingsMap curr_settings;
  InitDefaults(&curr_settings);
  if (!started_)
    settings_->SetDefaultValues(curr_settings);
  for (auto iter = curr_settings.begin(); iter != curr_settings.end(); ++iter)
    *iter = settings_->GetOption(iter.key());

  SettingsDialog dlg(curr_settings, local_time_ ? QTimeZone::systemTimeZone() : time_zone_);
  connect(&dlg, &SettingsDialog::destroyed, this, &TalkingClockPlugin::configured);

  // connect main signals/slots
  connect(&dlg, &SettingsDialog::OptionChanged, settings_, &PluginSettings::SetOption);
  connect(&dlg, &SettingsDialog::accepted, settings_, &PluginSettings::Save);
  connect(&dlg, &SettingsDialog::rejected, settings_, &PluginSettings::Load);

  connect(&dlg, &SettingsDialog::accepted, this, &TalkingClockPlugin::InitSpeechEngine);
  connect(&dlg, &SettingsDialog::rejected, this, &TalkingClockPlugin::InitSpeechEngine);

  dlg.exec();
}

void TalkingClockPlugin::SettingsListener(Option option, const QVariant& new_value)
{
  if (option == OPT_DISPLAY_LOCAL_TIME)
    local_time_ = new_value.toBool();

  if (option == OPT_TIME_ZONE)
    time_zone_ = QTimeZone(new_value.toByteArray());
}

void TalkingClockPlugin::TimeUpdateListener()
{
  if (!started_ || !speech_ || speech_->state() == QTextToSpeech::Speaking) return;

  QDateTime dt = QDateTime::currentDateTime();
  if (!local_time_) dt = dt.toTimeZone(time_zone_);
  QTime cur_time = dt.time();

  if (cur_time.minute() == 0 &&   // hour
      playback_allowed_ && settings_->GetOption(OPT_EVERY_HOUR_ENABLED).toBool()) {
    speech_->say(cur_time.toString(settings_->GetOption(OPT_EVERY_HOUR_FORMAT).toString()));
    playback_allowed_ = false;
  }

  if (cur_time.minute() % 15 == 0 && cur_time.minute() != 0 &&    // quarter
      playback_allowed_ && settings_->GetOption(OPT_QUARTER_HOUR_ENABLED).toBool()) {
    speech_->say(cur_time.toString(settings_->GetOption(OPT_QUARTER_HOUR_FORMAT).toString()));
    playback_allowed_ = false;
  }

  playback_allowed_ = (cur_time.minute() % 15 != 0);
}

void TalkingClockPlugin::InitSpeechEngine()
{
  if (!started_) return;

  delete speech_;

  QString engine_name = settings_->GetOption(OPT_SYNTHESIS_ENGINE).toString();
  if (engine_name == "default" || !QTextToSpeech::availableEngines().contains(engine_name))
    speech_ = new QTextToSpeech(this);
  else
    speech_ = new QTextToSpeech(engine_name, this);

  speech_->setVolume(settings_->GetOption(OPT_SYNTHESIS_VOLUME).toInt() / 100.0);
  speech_->setRate(settings_->GetOption(OPT_SYNTHESIS_RATE).toInt() / 100.0);
  speech_->setPitch(settings_->GetOption(OPT_SYNTHESIS_PITCH).toInt() / 100.0);

  QVector<QLocale> locales = speech_->availableLocales();
  int locale_index = settings_->GetOption(OPT_SYNTHESIS_LANGUAGE).toInt();
  if (locale_index >= 0 && locale_index < locales.size())
    speech_->setLocale(locales.at(locale_index));

  QVector<QVoice> voices = speech_->availableVoices();
  int voice_index = settings_->GetOption(OPT_SYNTHESIS_VOICE).toInt();
  if (voice_index >= 0 && voice_index < voices.size())
    speech_->setVoice(voices.at(voice_index));
}

} // namespace talking_clock
