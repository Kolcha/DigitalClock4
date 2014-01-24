#ifndef DATE_H
#define DATE_H

#include <QFont>
#include "iclock_plugin.h"

class QGridLayout;
class QLabel;
namespace skin_draw {
class SkinDrawer;
}
class PluginSettings;

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

private:
  QGridLayout* main_layout_;
  QWidget* main_wnd_;
  QPointer<QLabel> msg_label_;
  QFont font_;
  int avail_width_;
  qreal last_zoom_;
  QString last_date_;
  ::skin_draw::SkinDrawer* drawer_;
  PluginSettings* settings_;
};

} // namespace date

#endif // DATE_H
