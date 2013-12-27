#ifndef SPECTRUM_CLOCK_H
#define SPECTRUM_CLOCK_H

#include <QColor>
#include "iclock_plugin.h"

namespace spectrum_clock {

class SpectrumClock : public ISettingsPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID SETTINGS_PLUGIN_INTERFACE_IID FILE "spectrum_clock.json")
  Q_INTERFACES(IClockPlugin ISettingsPlugin)

public:
  SpectrumClock();
  void Init(const QMap<Options, QVariant>& current_settings);

public slots:
  void Start();
  void Stop();
  void Configure() {}
  void SettingsListener(Options, const QVariant&) {}
  void TimeUpdateListener();

private:
  QColor old_color_;
  QColor cur_color_;
};

} // namespace spectrum_clock

#endif // SPECTRUM_CLOCK_H
