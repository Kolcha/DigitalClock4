#ifndef HOUR_SIGNAL_H
#define HOUR_SIGNAL_H

#include "iclock_plugin.h"

class HourSignal : public IClockPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "hour_signal.json")
  Q_INTERFACES(IClockPlugin)

public:
  HourSignal() : last_hour_(-1) {}
  void GetInfo(TPluginInfo* info);

public slots:
  void Start() { emit started(); }
  void Stop() { emit stopped(); }
  void Configure() { emit configured(); }
  void SettingsListener(Options, const QVariant&) {}
  void TimeUpdateListener(const QString& current_time);

private:
  int last_hour_;
};

#endif // HOUR_SIGNAL_H