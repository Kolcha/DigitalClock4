#ifndef MOVE_CLOCK_MOVE_CLOCK_H
#define MOVE_CLOCK_MOVE_CLOCK_H

#include <QDesktopWidget>
#include <QTimer>
#include "iclock_plugin.h"
#include "iplugin_init.h"

class PluginSettings;

namespace move_clock {

class MoveClock : public IClockPlugin, public IWidgetPluginInit {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "move_clock.json")
  Q_INTERFACES(IClockPlugin IWidgetPluginInit)

public:
  MoveClock();
  void Init(QWidget* main_wnd);

public slots:
  void Start();
  void Stop();
  void Configure();
  void SettingsListener(Options, const QVariant&) {}
  void TimeUpdateListener() {}

private slots:
  void SettingsListener(const QString& key, const QVariant& value);
  void TimeoutHandler();

private:
  PluginSettings* settings_;
  QWidget* clock_wnd_;
  QDesktopWidget desktop_;
  QPoint old_pos_;
  QTimer timer_;
  double x0_, kx_, ax_, nx_;
  double y0_, ky_, ay_, ny_;
  double t_;
};

} // namespace move_clock

#endif // MOVE_CLOCK_MOVE_CLOCK_H
