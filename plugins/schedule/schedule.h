#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "iclock_plugin.h"

class Schedule : public IWidgetPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID WIDGET_PLUGIN_INTERFACE_IID FILE "schedule.json")
  Q_INTERFACES(IClockPlugin IWidgetPlugin)

public:
  Schedule();
  void Init(QWidget* main_wnd);

public slots:
  void Start();
  void Stop();
  void Configure();
  void SettingsListener(Options, const QVariant&) {}
  void TimeUpdateListener(const QString&);
};

#endif // SCHEDULE_H
