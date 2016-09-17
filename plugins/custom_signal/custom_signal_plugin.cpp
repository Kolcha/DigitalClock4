#include "custom_signal_plugin.h"

#include "plugin_settings.h"

#include "core/custom_signal_settings.h"
#include "core/signal_item.h"

#include "gui/settings_dialog.h"

namespace custom_signal {

CustomSignalPlugin::CustomSignalPlugin() : started_(false)
{
  InitTranslator(QLatin1String(":/custom_signal/custom_signal_"));
  info_.display_name = tr("Custom period signal");
  info_.description = tr("Plays sound with selected period."
                         "This plugin replaces all sound plugins:\n"
                         "\"Every hour signal\", \"Quarter of an hour signal\", \"Tower clock hour signal\".\n"
                         "Sounds from these plugins are included as default sounds.");
  InitIcon(":/custom_signal/icon.svg");

  player_ = new QMediaPlayer(this);
  connect(player_, &QMediaPlayer::stateChanged, this, &CustomSignalPlugin::onPlayerStateChanged);

  SignalItem* eh_signal = new SignalItem(this);
  eh_signal->setPeriod(60);
  eh_signal->setOffset(0);
  connect(eh_signal, &SignalItem::timeout, this, &CustomSignalPlugin::onSignalTriggered);
  all_signals_[SignalType::EveryHour] = eh_signal;
  SignalItem* qh_signal = new SignalItem(this);
  qh_signal->setPeriod(15);
  qh_signal->setOffset(0);
  connect(qh_signal, &SignalItem::timeout, this, &CustomSignalPlugin::onSignalTriggered);
  all_signals_[SignalType::QuarterHour] = qh_signal;
  SignalItem* cp_signal = new SignalItem(this);
  cp_signal->setPeriod(10);
  cp_signal->setOffset(0);
  connect(cp_signal, &SignalItem::timeout, this, &CustomSignalPlugin::onSignalTriggered);
  all_signals_[SignalType::CustomPeriod] = cp_signal;
}

void CustomSignalPlugin::Start()
{
  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
  settings_->TrackChanges(true);
  connect(settings_, &PluginSettings::OptionChanged, this, &CustomSignalPlugin::SettingsUpdateListener);
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
  SignalItem* c_item = all_signals_.value(SignalType::CustomPeriod, nullptr);
  Q_ASSERT(c_item);
  dialog->setNextCustomTime(c_item->next());
  // connect main signals/slots
  connect(dialog, SIGNAL(OptionChanged(QString,QVariant)),
          settings_, SLOT(SetOption(QString,QVariant)));
  connect(dialog, SIGNAL(accepted()), settings_, SLOT(Save()));
  connect(dialog, SIGNAL(rejected()), settings_, SLOT(Load()));
  connect(c_item, SIGNAL(nextChanged(QTime)), dialog, SLOT(setNextCustomTime(QTime)));
  dialog->show();
}

void CustomSignalPlugin::TimeUpdateListener()
{
  if (!started_) return;

  QTime now = QTime::currentTime();
  for (auto iter = all_signals_.begin(); iter != all_signals_.end(); ++iter)
    iter.value()->checkTime(now);
}

void CustomSignalPlugin::SettingsUpdateListener(const QString& key, const QVariant& value)
{
  if (key == OPT_EVERY_HOUR_ENABLED)
    all_signals_[SignalType::EveryHour]->setEnabled(value.toBool());
  if (key == OPT_EVERY_HOUR_SIGNAL)
    all_signals_[SignalType::EveryHour]->setSound(value.toUrl());
  if (key == OPT_EVERY_HOUR_VOLUME)
    all_signals_[SignalType::EveryHour]->setVolume(value.toInt());

  if (key == OPT_QUARTER_HOUR_ENABLED)
    all_signals_[SignalType::QuarterHour]->setEnabled(value.toBool());
  if (key == OPT_QUARTER_HOUR_SIGNAL)
    all_signals_[SignalType::QuarterHour]->setSound(value.toUrl());
  if (key == OPT_QUARTER_HOUR_VOLUME)
    all_signals_[SignalType::QuarterHour]->setVolume(value.toInt());

  if (key == OPT_CUSTOM_PERIOD_ENABLED)
    all_signals_[SignalType::CustomPeriod]->setEnabled(value.toBool());
  if (key == OPT_CUSTOM_PERIOD_SIGNAL)
    all_signals_[SignalType::CustomPeriod]->setSound(value.toUrl());
  if (key == OPT_CUSTOM_PERIOD_VOLUME)
    all_signals_[SignalType::CustomPeriod]->setVolume(value.toInt());
  if (key == OPT_CUSTOM_PERIOD_PERIOD)
    all_signals_[SignalType::CustomPeriod]->setPeriod(value.toInt());
  if (key == OPT_CUSTOM_PERIOD_OFFSET)
    all_signals_[SignalType::CustomPeriod]->setOffset(value.toInt());
}

void CustomSignalPlugin::onSignalTriggered()
{
  SignalItem* item = qobject_cast<SignalItem*>(sender());
  Q_ASSERT(item);
  Q_ASSERT(item->enabled());
  play_queue_.push_back(item);
  if (play_queue_.size() != 1) return;
  player_->setVolume(item->volume());
  player_->setMedia(item->sound());
  player_->play();
}

void CustomSignalPlugin::onPlayerStateChanged(QMediaPlayer::State new_state)
{
  if (play_queue_.empty()) return;

  if (new_state == QMediaPlayer::StoppedState) {
    player_->setMedia(QMediaContent());
    Q_ASSERT(!play_queue_.empty());
    play_queue_.pop_front();
    if (play_queue_.empty()) return;
    SignalItem* item = play_queue_.first();
    Q_ASSERT(item->enabled());
    player_->setVolume(item->volume());
    player_->setMedia(item->sound());
    player_->play();
  }
}

} // namespace custom_signal
