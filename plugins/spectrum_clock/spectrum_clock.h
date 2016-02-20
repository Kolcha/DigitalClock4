#ifndef SPECTRUM_CLOCK_H
#define SPECTRUM_CLOCK_H

#include <QColor>
#include "iclock_plugin.h"
#include "iplugin_init.h"

namespace spectrum_clock {

class SpectrumClock : public ISettingsPlugin, public ISettingsPluginInit {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "spectrum_clock.json")
  Q_INTERFACES(IClockPlugin ISettingsPlugin ISettingsPluginInit)

public:
  SpectrumClock();
  void Init(const QMap<Option, QVariant>& current_settings);

public slots:
  void Start();
  void Stop();
  void TimeUpdateListener();

private:
  QColor old_color_;
  QColor cur_color_;
};

} // namespace spectrum_clock

#endif // SPECTRUM_CLOCK_H
