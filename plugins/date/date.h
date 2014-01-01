#ifndef DATE_H
#define DATE_H

#include "iclock_plugin.h"

namespace date {

class Date : public IWidgetPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID WIDGET_PLUGIN_INTERFACE_IID FILE "date.json")
  Q_INTERFACES(IClockPlugin IWidgetPlugin)

public:
  Date();
  void Init(QWidget* main_wnd);

public slots:
  void Start();
  void Stop();
  void Configure();
  void SettingsListener(Options option, const QVariant& new_value);
  void TimeUpdateListener();
};

} // namespace date

#endif // DATE_H
