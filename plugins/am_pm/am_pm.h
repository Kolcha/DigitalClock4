#ifndef AM_PM_H
#define AM_PM_H

#include <QLabel>
#include <QGridLayout>
#include "iclock_plugin.h"

class ShowAMPM : public IWidgetPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID WIDGET_PLUGIN_INTERFACE_IID FILE "am_pm.json")
  Q_INTERFACES(IClockPlugin IWidgetPlugin)

public:
  void Init(QWidget* main_wnd);
  void GetInfo(TPluginInfo* info);

public slots:
  void Start();
  void Stop();
  void Configure() { emit configured(); }
  void SettingsListener(Options option, const QVariant& value);
  void TimeUpdateListener(const QString&);

private:
  QGridLayout* main_layout_;
  QLabel* am_pm_label_;
};

#endif // AM_PM_H
