#ifndef SPECTRUM_CLOCK_H
#define SPECTRUM_CLOCK_H

#include <QTimer>
#include "iclock_plugin.h"

class SpectrumClock : public ISettingsPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID SETTINGS_PLUGIN_INTERFACE_IID FILE "spectrum_clock.json")
  Q_INTERFACES(IClockPlugin ISettingsPlugin)

public:
  SpectrumClock();
  void Init(const QMap<Options, QVariant>& current_settings);
  void Configure() {}
  void Start();
  void Stop();
  void GetInfo(TPluginInfo* info);

public slots:
  void SettingsListener(Options, const QVariant&) {}

private slots:
  void TimeoutHandler();

private:
  QTimer timer_;
  QColor old_color_;
  QColor cur_color_;
};

#endif // SPECTRUM_CLOCK_H
