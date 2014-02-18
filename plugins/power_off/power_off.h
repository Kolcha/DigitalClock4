#ifndef POWER_OFF_H
#define POWER_OFF_H

#include "iclock_plugin.h"

class PluginSettings;

namespace power_off {

class PowerOff : public IClockPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "power_off.json")
  Q_INTERFACES(IClockPlugin)

public:
  PowerOff();

public slots:
  void Start();
  void Stop() {}
  void Configure();
  void SettingsListener(Options, const QVariant&) {}
  void TimeUpdateListener();

private:
  PluginSettings* settings_;
  bool active_;
};

} // namespace power_off

#endif // POWER_OFF_H