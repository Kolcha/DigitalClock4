#ifndef QUARTER_HOUR_SIGNAL_QUARTER_HOUR_SIGNAL_H
#define QUARTER_HOUR_SIGNAL_QUARTER_HOUR_SIGNAL_H

#include "iclock_plugin.h"

namespace quarter_hour_signal {

class QuarterHourSignal : public IClockPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "quarter_hour_signal.json")
  Q_INTERFACES(IClockPlugin)

public:
  QuarterHourSignal();

public slots:
  void TimeUpdateListener();

private:
  int last_minute_;
};

} // namespace quarter_hour_signal

#endif // QUARTER_HOUR_SIGNAL_QUARTER_HOUR_SIGNAL_H
