#ifndef ANY_ZOOM_H
#define ANY_ZOOM_H

#include <QPointer>
#include <QInputDialog>
#include "iclock_plugin.h"
#include "plugin_settings.h"

class AnyZoom : public ISettingsPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID SETTINGS_PLUGIN_INTERFACE_IID FILE "any_zoom.json")
  Q_INTERFACES(IClockPlugin ISettingsPlugin)

public:
  AnyZoom();

  void Init(const QMap<Options, QVariant>& current_settings, QWidget* parent);
  void GetInfo(TPluginInfo* info);

public slots:
  void Start();
  void Stop();
  void Configure();
  void SettingsListener(Options, const QVariant&) {}
  void TimeUpdateListener(const QString&) {}

private slots:
  void TrackChange(int new_zoom);
  void RevertSettings();

private:
  QWidget* parent_;
  PluginSettings* settings_;
  QPointer<QInputDialog> settings_dlg_;
  bool is_enabled_;
};

#endif // ANY_ZOOM_H
