#ifndef ANY_ZOOM_H
#define ANY_ZOOM_H

#include "iclock_plugin.h"
#include "iplugin_init.h"

class PluginSettings;

namespace any_zoom {

class AnyZoom : public ISettingsPlugin, ISettingsPluginInit {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "any_zoom.json")
  Q_INTERFACES(IClockPlugin ISettingsPlugin ISettingsPluginInit)

public:
  AnyZoom();
  void Init(const QMap<Option, QVariant>& current_settings);

  void ExportSettings(QSettings::SettingsMap* settings);
  void ImportSettings(const QSettings::SettingsMap& settings);

public slots:
  void Start();
  void Stop();
  void Configure();
  void SettingsListener(Option option, const QVariant& value);

private slots:
  void TrackChange(int new_zoom);
  void RevertSettings();

private:
  PluginSettings* settings_;
  bool is_enabled_;
  qreal last_zoom_;
};

} // namespace any_zoom

#endif // ANY_ZOOM_H
