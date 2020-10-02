/*
    Digital Clock: timetracker plugin
    Copyright (C) 2018-2020  Nick Korotysh <nick.korotysh@gmail.com>

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
#include "timetracker_plugin.h"

#include <QCoreApplication>
#include <QGridLayout>

#ifdef HAVE_QHOTKEY
#include <QHotkey>
#else
class QHotkey {};   // just a stub to suppress compiler warnings
#endif

#include "plugin_settings.h"
#include "widget_plugin_settings.h"

#include "timetracker.h"
#include "tracker_widget.h"

#include "core/settings.h"
#include "gui/settings_dialog.h"

namespace timetracker {

// THIS IS A TRICK!
// these variables exist just to survive after plugins
// reload on settings changes (it happens in any case)
// but plugin state is reset after application restart
static const char* const PROP_STATE_ELAPSED = "dcp_timetracker_state_last_elapsed";
static const char* const PROP_STATE_ACTIVE = "dcp_timetracker_state_last_active";

TimetrackerPlugin::TimetrackerPlugin() : tracker_(nullptr)
  , pause_hotkey_(nullptr)
  , restart_hotkey_(nullptr)
{
  InitTranslator(QLatin1String(":/timetracker/timetracker_"));
  info_.display_name = tr("Stopwatch");
  info_.description = tr("Very simple stopwatch.\n"
                         "Single click to start/pause.\n"
                         "Double click to reset/restart.");
//  InitIcon(":/timetracker/icon.svg.p");
}

void TimetrackerPlugin::Start()
{
  if (tracker_) return;
  tracker_ = new Timetracker();
  // TRICK! restore state from dynamic app properties
  QVariant prop_var = QCoreApplication::instance()->property(PROP_STATE_ELAPSED);
  if (prop_var.isValid()) tracker_->setElapsed(prop_var.toInt());
  prop_var = QCoreApplication::instance()->property(PROP_STATE_ACTIVE);
  if (prop_var.isValid() && prop_var.toBool()) tracker_->start();
  connect(settings_, &PluginSettings::OptionChanged, this, &TimetrackerPlugin::onPluginOptionChanged);
  // state is restored before widget initialization to prevent flickering
  ::plugin::WidgetPluginBase::Start();
  settings_->SetOption(::plugin::OptionKey(::plugin::OPT_USE_CLOCK_SKIN), true);
}

void TimetrackerPlugin::Stop()
{
  ::plugin::WidgetPluginBase::Stop();
  if (tracker_) {
    // TRICK! save state to dynamic app properties
    QCoreApplication::instance()->setProperty(PROP_STATE_ACTIVE, tracker_->isActive());
    QCoreApplication::instance()->setProperty(PROP_STATE_ELAPSED, tracker_->elapsed());
    tracker_->stop();
  }
  delete tracker_;
  tracker_ = nullptr;

  delete pause_hotkey_;
  delete restart_hotkey_;

  timer_widgets_.clear();
}

void TimetrackerPlugin::Configure()
{
  SettingsDialog* dialog = new SettingsDialog();
  connect(dialog, &SettingsDialog::destroyed, this, &TimetrackerPlugin::configured);
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

  dialog->show();
}

void TimetrackerPlugin::InitSettingsDefaults(QSettings::SettingsMap* defaults)
{
  InitDefaults(defaults);
}

QWidget* TimetrackerPlugin::InitWidget(QGridLayout* layout)
{
  Q_UNUSED(layout);
  TrackerWidget* w = new TrackerWidget();
  connect(w, &TrackerWidget::clicked, this, &TimetrackerPlugin::onWidgetClicked);
  connect(w, &TrackerWidget::doubleClicked, tracker_, &Timetracker::reset);
  timer_widgets_.append(w);
  return w;
}

void TimetrackerPlugin::DisplayImage(QWidget* widget, const QImage& image)
{
  static_cast<TrackerWidget*>(widget)->setPixmap(QPixmap::fromImage(image));
}

QString TimetrackerPlugin::GetWidgetText()
{
  int s = tracker_->elapsed();
  int h = s / 3600;
  s -= h * 3600;
  int m = s / 60;
  s -= m * 60;
  return QString("%1:%2:%3").arg(h).arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0'));
}

void TimetrackerPlugin::onWidgetClicked()
{
//  using namespace ::plugin;
  if (tracker_->isActive()) {
    tracker_->stop();
//    settings_->SetOption(OptionKey(OPT_USE_CUSTOM_COLOR), true);
  } else {
    tracker_->start();
//    settings_->SetOption(OptionKey(OPT_USE_CUSTOM_COLOR), false);
  }
}

void TimetrackerPlugin::setWidgetsVisible(bool visible)
{
  for (auto w : timer_widgets_)
    if (w)
      w->setVisible(visible);
}

void TimetrackerPlugin::onPluginOptionChanged(const QString& key, const QVariant& value)
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
    pause_hotkey_ = init_hotkey(value.toString(), this, &TimetrackerPlugin::onWidgetClicked);
  }

  if (key == OPT_RESTART_HOTKEY) {
    delete restart_hotkey_;
    restart_hotkey_ = init_hotkey(value.toString(), tracker_, &Timetracker::reset);
  }

  if (key == OPT_HIDE_INACTIVE && tracker_) {
    if (value.toBool()) {
      connect(tracker_, &Timetracker::activityChanged, this, &TimetrackerPlugin::setWidgetsVisible);
      setWidgetsVisible(tracker_->isActive());
    } else {
      disconnect(tracker_, &Timetracker::activityChanged, this, &TimetrackerPlugin::setWidgetsVisible);
      setWidgetsVisible(true);
    }
  }
}

} // namespace timetracker
