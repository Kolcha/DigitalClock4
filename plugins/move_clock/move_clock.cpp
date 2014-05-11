#include <cmath>
#include <QWidget>
#include "plugin_settings.h"
#include "move_clock_settings.h"
#include "gui/settings_dialog.h"
#include "move_clock.h"

namespace move_clock {

double x(double c, double k, double a, double n, double t) {
  return c + k*t + a*sin(n*t);
}

double y(double c, double k, double a, double n, double t) {
  return c + k*t + a*cos(n*t);
}

MoveClock::MoveClock() {
  settings_ = new PluginSettings("Nick Korotysh", "Digital Clock", this);
  connect(settings_, SIGNAL(OptionChanged(QString,QVariant)),
          this, SLOT(SettingsListener(QString,QVariant)));

  timer_.setSingleShot(false);
  connect(&timer_, SIGNAL(timeout()), this, SLOT(TimeoutHandler()));

  InitTranslator(QLatin1String(":/move_clock/move_clock_"));
  info_.display_name = tr("Move Clock");
  info_.description = tr("Moves the clock.");
  info_.icon.load(":/move_clock/icon.png");
}

void MoveClock::Init(QWidget* main_wnd) {
  clock_wnd_ = main_wnd;
  old_pos_ = main_wnd->pos();

  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
  settings_->TrackChanges(true);
}

void MoveClock::Start() {
  settings_->Load();
  timer_.start();
}

void MoveClock::Stop() {
  timer_.stop();
  clock_wnd_->move(old_pos_);
}

void MoveClock::Configure() {
  SettingsDialog* dialog = new SettingsDialog();
  // load current settings to dialog
  connect(settings_, SIGNAL(OptionChanged(QString,QVariant)),
          dialog, SLOT(SettingsListener(QString,QVariant)));
  settings_->Load();
  // connect main signals/slots
  connect(dialog, SIGNAL(OptionChanged(QString,QVariant)),
          settings_, SLOT(SetOption(QString,QVariant)));
  connect(dialog, SIGNAL(accepted()), settings_, SLOT(Save()));
  connect(dialog, SIGNAL(rejected()), settings_, SLOT(Load()));
  dialog->show();
}

void MoveClock::SettingsListener(const QString& key, const QVariant& value) {
  if (key == OPT_TIMEOUT) timer_.setInterval(value.toInt());
}

void MoveClock::TimeoutHandler() {
}

} // namespace move_clock
