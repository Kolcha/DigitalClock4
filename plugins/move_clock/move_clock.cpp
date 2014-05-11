#include <QWidget>
#include "plugin_settings.h"
#include "move_clock.h"

namespace move_clock {

MoveClock::MoveClock() {
  settings_ = new PluginSettings("Nick Korotysh", "Digital Clock", this);
  connect(settings_, SIGNAL(OptionChanged(QString,QVariant)),
          this, SLOT(SettingsListener(QString,QVariant)));

  InitTranslator(QLatin1String(":/move_clock/move_clock_"));
  info_.display_name = tr("Move Clock");
  info_.description = tr("Moves the clock.");
  info_.icon.load(":/move_clock/icon.png");
}

void MoveClock::Init(QWidget* main_wnd) {
  clock_wnd_ = main_wnd;
  old_pos_ = main_wnd->pos();
}

void MoveClock::Start() {

}

void MoveClock::Stop() {
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

void MoveClock::TimeUpdateListener() {
}

void MoveClock::SettingsListener(const QString& key, const QVariant& value) {
}

} // namespace move_clock
