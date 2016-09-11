#ifndef CUSTOM_SIGNAL_CUSTOM_SIGNAL_PLUGIN_H
#define CUSTOM_SIGNAL_CUSTOM_SIGNAL_PLUGIN_H

#include "iclock_plugin.h"

namespace custom_signal {

class CustomSignalPlugin : public IClockPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "custom_signal.json")
  Q_INTERFACES(IClockPlugin)

public:
  CustomSignalPlugin();

public slots:
  void Configure();

  void TimeUpdateListener();
};

} // namespace custom_signal

#endif // CUSTOM_SIGNAL_CUSTOM_SIGNAL_PLUGIN_H
