#ifndef ICLOCK_PLUGIN_H
#define ICLOCK_PLUGIN_H

#include <QtPlugin>
#include "settings_keys.h"

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


class IClockPlugin : public QObject {
  Q_OBJECT

public:
  virtual ~IClockPlugin() {}
  virtual void Start() = 0;
  virtual void Stop() = 0;
  virtual void Configure() = 0;
  virtual void GetInfo(TPluginInfo* info) = 0;

public slots:
  virtual void SettingsListener(Options option, const QVariant& new_value) = 0;
};


class ISettingsPlugin : public IClockPlugin {
  Q_OBJECT

public:
  virtual ~ISettingsPlugin() {}
  virtual void Init(const QMap<Options, QVariant>& current_settings) = 0;

signals:
  void OptionChanged(Options option, const QVariant& value);
};

#define SETTINGS_PLUGIN_INTERFACE_IID   "nick-korotysh.digital-clock.settings-plugin"
Q_DECLARE_INTERFACE(ISettingsPlugin, SETTINGS_PLUGIN_INTERFACE_IID)

#endif // ICLOCK_PLUGIN_H
