#ifndef SPECTRUM_CLOCK_H
#define SPECTRUM_CLOCK_H

#include <QColor>
#include "core/iclock_plugin.h"

class SpectrumClock : public QObject, public IClockPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID PLUGIN_INTERFACE_IID FILE "spectrum_clock.json")
  Q_INTERFACES(IClockPlugin)

public:
  SpectrumClock();
  void Init(const TPluginData& data);
  void Config();
  void Start();
  void Stop();
  const TPluginInfo& GetInfo() const;

private slots:
  void TimeoutHandler(const QString&);

private:
  TPluginInfo info_;
  TPluginData data_;
  QColor old_color_;
  QColor cur_color_;
};

#endif // SPECTRUM_CLOCK_H
