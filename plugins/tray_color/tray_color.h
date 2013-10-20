#ifndef TRAY_COLOR_H
#define TRAY_COLOR_H

#include "iclock_plugin.h"
#include "plugin_settings.h"

class TrayColor : public ITrayPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID TRAY_PLUGIN_INTERFACE_IID FILE "tray_color.json")
  Q_INTERFACES(IClockPlugin ITrayPlugin)

public:
  TrayColor();
  void Init(QSystemTrayIcon* tray_icon, QWidget* parent);
  void GetInfo(TPluginInfo* info);

public slots:
  void Start();
  void Stop();
  void Configure();
  void SettingsListener(Options, const QVariant&) {}
  void TimeUpdateListener(const QString&) {}

private:
  void RedrawTrayIcon(const QColor& color);

  QSystemTrayIcon* tray_icon_;
  QWidget* parent_;
  PluginSettings* settings_;

  bool is_enabled_;
};

#endif // TRAY_COLOR_H
