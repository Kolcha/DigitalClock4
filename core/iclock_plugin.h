#ifndef ICLOCK_PLUGIN_H
#define ICLOCK_PLUGIN_H

#include <QtPlugin>

class ClockSettings;
class DigitalClock;
class MainWindow;
class TrayControl;

// data that available for plugins
struct TPluginData {
  ClockSettings* settings;   // clock settings object
  DigitalClock* clock;       // clock widget
  MainWindow* window;        // main window instance
  TrayControl* tray;         // tray control object
};

// plugin info fields
enum FPluginInfo {
  PI_NAME,        // plugin name
  PI_VERSION,     // plugin version
  PI_AUTHOR,      // plugin author
  PI_EMAIL,       // author's e-mail
  PI_COMMENT      // plugin description
};
// plugin info type
typedef QMap<FPluginInfo, QString> TPluginInfo;

class IClockPlugin {
public:
  virtual ~IClockPlugin() {}

  virtual void Init(const TPluginData& data) = 0;
  virtual void Config() = 0;
  virtual void Start() = 0;
  virtual void Stop() = 0;
  virtual const TPluginInfo& GetInfo() const = 0;
};

#define PLUGIN_INTERFACE_IID   "nick-korotysh.digital-clock.plugin"
Q_DECLARE_INTERFACE(IClockPlugin, PLUGIN_INTERFACE_IID)

#endif // ICLOCK_PLUGIN_H
