#ifndef ANY_ZOOM_H
#define ANY_ZOOM_H

#include "iclock_plugin.h"

class PluginSettings;

class AnyZoom : public ISettingsPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID SETTINGS_PLUGIN_INTERFACE_IID FILE "any_zoom.json")
  Q_INTERFACES(IClockPlugin ISettingsPlugin)

public:
  AnyZoom();
  void Init(const QMap<Options, QVariant>& current_settings, QWidget* parent);

public slots:
  void Start();
  void Stop();
  void Configure();
  void SettingsListener(Options option, const QVariant& value);
  void TimeUpdateListener(const QString&) {}

private slots:
  void TrackChange(int new_zoom);
  void RevertSettings();

private:
  QWidget* parent_;
  PluginSettings* settings_;
  bool is_enabled_;
  qreal last_zoom_;
};

#endif // ANY_ZOOM_H
