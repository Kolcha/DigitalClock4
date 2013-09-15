#ifndef AM_PM_H
#define AM_PM_H

#include <QLabel>
#include <QGridLayout>
#include "iclock_plugin.h"
#include "skin_drawer.h"

class ShowAMPM : public IWidgetPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID WIDGET_PLUGIN_INTERFACE_IID FILE "am_pm.json")
  Q_INTERFACES(IClockPlugin IWidgetPlugin)

public:
  ShowAMPM();
  void Init(QWidget* main_wnd);
  void GetInfo(TPluginInfo* info);

public slots:
  void Start();
  void Stop();
  void Configure() { emit configured(); }
  void SettingsListener(Options option, const QVariant& value);
  void TimeUpdateListener(const QString&);

private slots:
  void SetImage(const QImage& img);

private:
  void SetNewZoom();

  QGridLayout* main_layout_;
  QWidget* main_widget_;
  QLabel* am_pm_label_;
  SkinDrawer* drawer_;
  QFont font_;
};

#endif // AM_PM_H
