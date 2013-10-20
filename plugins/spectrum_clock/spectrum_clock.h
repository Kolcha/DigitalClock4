#ifndef SPECTRUM_CLOCK_H
#define SPECTRUM_CLOCK_H

#include "iclock_plugin.h"

class SpectrumClock : public ISettingsPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID SETTINGS_PLUGIN_INTERFACE_IID FILE "spectrum_clock.json")
  Q_INTERFACES(IClockPlugin ISettingsPlugin)

public:
  void Init(const QMap<Options, QVariant>& current_settings, QWidget*);
  void GetInfo(TPluginInfo* info);

public slots:
  void Start();
  void Stop();
  void Configure() {}
  void SettingsListener(Options, const QVariant&) {}
  void TimeUpdateListener(const QString&);

private:
  QColor old_color_;
  QColor cur_color_;
};

#endif // SPECTRUM_CLOCK_H
