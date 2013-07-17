#ifndef ICLOCK_PLUGIN_H
#define ICLOCK_PLUGIN_H

#include <QtPlugin>

class ClockSettings;
class DigitalClock;
class MainWindow;
class TrayControl;

struct TPluginData {
  ClockSettings* settings;
  DigitalClock* clock;
  MainWindow* window;
  TrayControl* tray;
};

class IClockPlugin {
public:
  virtual ~IClockPlugin() {}

  virtual void Init(const TPluginData& data) = 0;
  virtual void Start() = 0;
  virtual void Stop() = 0;
};

#define PLUGIN_INTERFACE_IID   "nick-korotysh.digital-clock.plugin"
Q_DECLARE_INTERFACE(IClockPlugin, PLUGIN_INTERFACE_IID)

#endif // ICLOCK_PLUGIN_H
