/*
    Digital Clock: countdown timer plugin
    Copyright (C) 2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#include <QLabel>
#include <QVBoxLayout>
#include <QDateTime>

#include "plugin_settings.h"

#include "core/countdown_timer.h"
#include "core/settings.h"
#include "core/utilities.h"
#include "gui/settings_dialog.h"

namespace countdown_timer {

CountdownTimerPlugin::CountdownTimerPlugin() : msg_label_(nullptr), cd_timer_(nullptr)
{
  InitTranslator(QLatin1String(":/countdown_timer/countdown_timer_"));
  info_.display_name = tr("Countdown timer");
  info_.description = tr("Just a countdown timer.");
  InitIcon(":/countdown_timer/icon.svg.p");
  plg_name_ = QString("countdown_timer");
}

void CountdownTimerPlugin::Start()
{
    cd_timer_ = new CountdownTimer();
    connect(cd_timer_, &CountdownTimer::timeLeftChanged, this, &CountdownTimerPlugin::TimeUpdateListener);

    ::plugin::WidgetPluginBase::Start();
    InitTimer();
}

void CountdownTimerPlugin::Stop()
{
    if (cd_timer_->isActive()) cd_timer_->stop();
    delete cd_timer_;
    cd_timer_ = nullptr;

    ::plugin::WidgetPluginBase::Stop();
}

void CountdownTimerPlugin::Configure()
{
  SettingsDialog* dialog = new SettingsDialog();
  // load current settings to dialog
  QSettings::SettingsMap curr_settings;
  InitDefaults(&curr_settings);
  for (auto iter = curr_settings.begin(); iter != curr_settings.end(); ++iter) {
    *iter = settings_->GetOption(iter.key());
  }
  dialog->Init(curr_settings);
  // add widget with common settings configuration controls
  QVBoxLayout* dlg_layout = qobject_cast<QVBoxLayout*>(dialog->layout());
  Q_ASSERT(dlg_layout);
  dlg_layout->insertWidget(dlg_layout->count() - 1, InitConfigWidget(dialog));
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
  msg_label_ = new QLabel();
  return msg_label_;
}

void CountdownTimerPlugin::DisplayImage(const QImage& image)
{
  msg_label_->setPixmap(QPixmap::fromImage(image));
}

QString CountdownTimerPlugin::GetWidgetText()
{
    return format_time(cd_timer_->timeLeft());
}

void CountdownTimerPlugin::InitTimer()
{
    if (settings_->GetOption(OPT_USE_TARGET_TIME).toBool()) {
        QDateTime now = QDateTime::currentDateTime();
        now = now.addMSecs(-now.time().msec());
        QDateTime target = settings_->GetOption(OPT_TARGET_DATETIME).toDateTime();
        if (target > now) {
            cd_timer_->setTimeout(now.secsTo(target));
            cd_timer_->start();
        }
    } else {
        qint64 timeout = settings_->GetOption(OPT_INTERVAL_SECONDS).toLongLong();
        timeout += 60 * settings_->GetOption(OPT_INTERVAL_MINUTES).toLongLong();
        timeout += 3600 * settings_->GetOption(OPT_INTERVAL_HOURS).toLongLong();
        cd_timer_->setTimeout(timeout);
    }
}

} // namespace countdown_timer
