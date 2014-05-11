#ifndef MOVE_CLOCK_MOVE_CLOCK_H
#define MOVE_CLOCK_MOVE_CLOCK_H

#include "iclock_plugin.h"
#include "iplugin_init.h"

class PluginSettings;

namespace move_clock {

class MoveClock : public IClockPlugin, public IWidgetPluginInit {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "move_clock.json")

public:
  MoveClock();
  void Init(QWidget* main_wnd);

public slots:
  void Start();
  void Stop();
  void Configure();
  void SettingsListener(Options option, const QVariant& new_value) {}
  void TimeUpdateListener();

private slots:
  void SettingsListener(const QString& key, const QVariant& value);

private:
  PluginSettings* settings_;
  QWidget* clock_wnd_;
  QPoint old_pos_;
};

} // namespace move_clock

#endif // MOVE_CLOCK_MOVE_CLOCK_H
