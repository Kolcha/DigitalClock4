#include "custom_signal_plugin.h"

#include "plugin_settings.h"

#include "core/custom_signal_settings.h"

#include "gui/settings_dialog.h"

namespace custom_signal {

CustomSignalPlugin::CustomSignalPlugin() : started_(false)
{
  InitTranslator(QLatin1String(":/custom_signal/custom_signal_"));
  info_.display_name = tr("Custom period signal");
  info_.description = tr("Plays sound with selected period.");
  InitIcon(":/custom_signal/icon.svg");

  player_ = new QMediaPlayer(this);
}

void CustomSignalPlugin::Start()
{
  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
  settings_->TrackChanges(true);
  settings_->Load();
  started_ = true;
}

void CustomSignalPlugin::Stop()
{
  started_ = false;
  player_->stop();
}

void CustomSignalPlugin::Configure()
{
  SettingsDialog* dialog = new SettingsDialog();
  // load current settings to dialog
  QSettings::SettingsMap curr_settings;
  InitDefaults(&curr_settings);
  if (!started_) settings_->SetDefaultValues(curr_settings);
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

void CustomSignalPlugin::TimeUpdateListener()
{
  if (!started_) return;
}

} // namespace custom_signal
