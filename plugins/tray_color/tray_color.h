#ifndef TRAY_COLOR_H
#define TRAY_COLOR_H

#include "iclock_plugin.h"
#include "iplugin_init.h"

class PluginSettings;

namespace tray_color {

class TrayColor : public IClockPlugin, public ITrayPluginInit {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "tray_color.json")
  Q_INTERFACES(IClockPlugin ITrayPluginInit)

public:
  TrayColor();
  void Init(QSystemTrayIcon* tray_icon);

  void ExportSettings(QSettings::SettingsMap* settings);
  void ImportSettings(const QSettings::SettingsMap& settings);

public slots:
  void Start();
  void Stop();
  void Configure();
  void SettingsListener(Option opt, const QVariant& value);

private:
  void RedrawTrayIcon(const QColor& color);

  QSystemTrayIcon* tray_icon_;
  PluginSettings* settings_;

  bool is_enabled_;
};

} // namespace tray_color

#endif // TRAY_COLOR_H
