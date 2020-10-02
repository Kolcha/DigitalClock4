/*
    Digital Clock: countdown timer plugin
    Copyright (C) 2017-2020  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "countdown_timer_plugin.h"

#include <QDateTime>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QPushButton>

#ifdef HAVE_QHOTKEY
#include <QHotkey>
#else
class QHotkey {};   // just a stub to suppress compiler warnings
#endif

#include "plugin_settings.h"

#include "core/countdown_timer.h"
#include "core/settings.h"
#include "core/utilities.h"
#include "gui/clickable_label.h"
#include "gui/settings_dialog.h"

namespace countdown_timer {

CountdownTimerPlugin::CountdownTimerPlugin()
  : cd_timer_(nullptr)
  , player_(nullptr)
  , pause_hotkey_(nullptr)
  , restart_hotkey_(nullptr)
{
  InitTranslator(QLatin1String(":/countdown_timer/lang/countdown_timer_"));
  info_.display_name = tr("Countdown timer");
  info_.description = tr("Just a countdown timer.");
  InitIcon(":/countdown_timer/icon.svg.p");
}

void CountdownTimerPlugin::Start()
{
  cd_timer_ = new CountdownTimer();
  connect(cd_timer_, &CountdownTimer::timeLeftChanged, this, &CountdownTimerPlugin::TimeUpdateListener);
  connect(cd_timer_, &CountdownTimer::timeout, this, &CountdownTimerPlugin::HandleTimeout);

  player_ = new QMediaPlayer();

  connect(settings_, &PluginSettings::OptionChanged, this, &CountdownTimerPlugin::onPluginOptionChanged);

  ::plugin::WidgetPluginBase::Start();
  InitTimer();
}

void CountdownTimerPlugin::Stop()
{
  if (cd_timer_->isActive()) cd_timer_->stop();
  delete cd_timer_;
  cd_timer_ = nullptr;

  delete player_;
  player_ = nullptr;

  delete pause_hotkey_;
  delete restart_hotkey_;

  timer_widgets_.clear();

  ::plugin::WidgetPluginBase::Stop();
}

void CountdownTimerPlugin::Configure()
{
  SettingsDialog* dialog = new SettingsDialog();
  connect(dialog, &SettingsDialog::destroyed, this, &CountdownTimerPlugin::configured);
  // load current settings to dialog
  QSettings::SettingsMap curr_settings;
  InitDefaults(&curr_settings);
  for (auto iter = curr_settings.begin(); iter != curr_settings.end(); ++iter) {
    *iter = settings_->GetOption(iter.key());
  }
  dialog->Init(curr_settings);
  // add widget with common settings configuration controls
  dialog->AddCommonWidget(InitConfigWidget(dialog));
  // connect main signals/slots
  connect(dialog, SIGNAL(OptionChanged(QString,QVariant)),
          settings_, SLOT(SetOption(QString,QVariant)));
  connect(dialog, SIGNAL(accepted()), settings_, SLOT(Save()));
  connect(dialog, SIGNAL(rejected()), settings_, SLOT(Load()));
  // connect additional slots
  if (cd_timer_) {
    connect(dialog, &SettingsDialog::accepted, cd_timer_, &CountdownTimer::stop);
    connect(dialog, &SettingsDialog::accepted, this, &CountdownTimerPlugin::InitTimer);
    connect(dialog, &SettingsDialog::accepted, cd_timer_, &CountdownTimer::start);
  }
  dialog->show();
}

void CountdownTimerPlugin::InitSettingsDefaults(QSettings::SettingsMap* defaults)
{
  InitDefaults(defaults);
}

QWidget* CountdownTimerPlugin::InitWidget(QGridLayout* layout)
{
  Q_UNUSED(layout);
  ClickableLabel* w = new ClickableLabel();
  connect(w, &ClickableLabel::clicked, this, &CountdownTimerPlugin::RestartTimer);
  connect(w, &ClickableLabel::singleClicked, this, &CountdownTimerPlugin::PauseTimer);
  timer_widgets_.append(w);
  return w;
}

void CountdownTimerPlugin::DisplayImage(QWidget* widget, const QImage& image)
{
  static_cast<ClickableLabel*>(widget)->setPixmap(QPixmap::fromImage(image));
}

QString CountdownTimerPlugin::GetWidgetText()
{
  return format_time(cd_timer_->timeLeft(), settings_->GetOption(OPT_HIDE_DAYS_THRESHOLD).toInt());
}

void CountdownTimerPlugin::InitTimer()
{
  if (settings_->GetOption(OPT_USE_TARGET_TIME).toBool()) {
    QDateTime now = QDateTime::currentDateTime();
    now = now.addMSecs(-now.time().msec());
    QDateTime target = settings_->GetOption(OPT_TARGET_DATETIME).toDateTime();
    if (target < now) {
      target = GetDefaultDate();
      settings_->SetOption(OPT_TARGET_DATETIME, target);
    }
    if (target > now) {
      cd_timer_->setInterval(now.secsTo(target));
      cd_timer_->start();
    }
  } else {
    qint64 timeout = settings_->GetOption(OPT_INTERVAL_SECONDS).toLongLong();
    timeout += 60 * settings_->GetOption(OPT_INTERVAL_MINUTES).toLongLong();
    timeout += 3600 * settings_->GetOption(OPT_INTERVAL_HOURS).toLongLong();
    cd_timer_->setInterval(timeout);
  }
}

void CountdownTimerPlugin::HandleTimeout()
{
  if (settings_->GetOption(OPT_CHIME_ON_TIMEOUT).toBool()) {
    player_->setMedia(QUrl::fromLocalFile(settings_->GetOption(OPT_CHIME_SOUND_FILE).toString()));
    player_->play();
  }

  if (settings_->GetOption(OPT_SHOW_MESSAGE).toBool()) {
    QMessageBox mb(QMessageBox::Warning,
                   info_.display_name,
                   settings_->GetOption(OPT_MESSAGE_TEXT).toString());
    mb.addButton(QMessageBox::Ok)->setFocusPolicy(Qt::ClickFocus);
    mb.exec();
  }

  if (settings_->GetOption(OPT_RESTART_ON_TIMEOUT).toBool()) {
    InitTimer();
    cd_timer_->start();
  }
}

void CountdownTimerPlugin::RestartTimer()
{
  if (settings_->GetOption(OPT_RESTART_ON_DBLCLIK).toBool()) {
    cd_timer_->stop();
    InitTimer();
    cd_timer_->start();
  }
}

void CountdownTimerPlugin::PauseTimer()
{
  if (cd_timer_->isActive())
    cd_timer_->stop();
  else
    cd_timer_->start();
}

void CountdownTimerPlugin::setWidgetsVisible(bool visible)
{
  for (auto w : timer_widgets_)
    if (w)
      w->setVisible(visible);
}

void CountdownTimerPlugin::onPluginOptionChanged(const QString& key, const QVariant& value)
{
  auto init_hotkey = [=](auto key_seq, auto receiver, auto method) -> QHotkey* {
    QHotkey* hotkey = nullptr;
#ifdef HAVE_QHOTKEY
    if (!key_seq.isEmpty()) {
      hotkey = new QHotkey(QKeySequence(key_seq), true);
      connect(hotkey, &QHotkey::activated, receiver, method);
    }
#else
    Q_UNUSED(key_seq);
    Q_UNUSED(receiver);
    Q_UNUSED(method);
#endif
    return hotkey;
  };

  if (key == OPT_PAUSE_HOTKEY) {
    delete pause_hotkey_;
    pause_hotkey_ = init_hotkey(value.toString(), this, &CountdownTimerPlugin::PauseTimer);
  }

  if (key == OPT_RESTART_HOTKEY) {
    delete restart_hotkey_;
    restart_hotkey_ = init_hotkey(value.toString(), this, &CountdownTimerPlugin::RestartTimer);
  }

  if (key == OPT_HIDE_INACTIVE && cd_timer_) {
    if (value.toBool()) {
      connect(cd_timer_, &CountdownTimer::activityChanged, this, &CountdownTimerPlugin::setWidgetsVisible);
      setWidgetsVisible(cd_timer_->isActive());
    } else {
      disconnect(cd_timer_, &CountdownTimer::activityChanged, this, &CountdownTimerPlugin::setWidgetsVisible);
      setWidgetsVisible(true);
    }
  }
}

} // namespace countdown_timer
