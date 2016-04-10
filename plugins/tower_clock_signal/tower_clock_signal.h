#ifndef TOWER_CLOCK_SIGNAL_H
#define TOWER_CLOCK_SIGNAL_H

#include "iclock_plugin.h"


class QSound;

namespace tower_clock_signal {

class TowerCLockSignal : public IClockPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "tower_clock_signal.json")
  Q_INTERFACES(IClockPlugin)

public:
  TowerCLockSignal();

public slots:
  void Stop();

  void TimeUpdateListener();

private:
  QSound* bell_sound_;
};

} // namespace tower_clock_signal

#endif // TOWER_CLOCK_SIGNAL_H
