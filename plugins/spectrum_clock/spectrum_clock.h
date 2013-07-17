#ifndef SPECTRUM_CLOCK_H
#define SPECTRUM_CLOCK_H

#include "core/iclock_plugin.h"

class SpectrumClock : public QObject, public IClockPlugin {
  Q_OBJECT

public:
  SpectrumClock(QObject *parent = 0);
};

#endif // SPECTRUM_CLOCK_H
