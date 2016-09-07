#ifndef HOUR_SIGNAL_H
#define HOUR_SIGNAL_H

#include "iclock_plugin.h"

namespace hour_signal {

class HourSignal : public IClockPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "hour_signal.json")
  Q_INTERFACES(IClockPlugin)

public:
  HourSignal();

public slots:
  void TimeUpdateListener();

private:
  int last_hour_;
};

} // namespace hour_signal

#endif // HOUR_SIGNAL_H
