#ifndef ALARM_H
#define ALARM_H

#include <QIcon>
#include <QPointer>
#include <QMediaPlayer>
#include "iclock_plugin.h"
#include "iplugin_init.h"

class PluginSettings;

namespace alarm {

class Alarm : public IClockPlugin, public ITrayPluginInit {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "alarm.json")
  Q_INTERFACES(IClockPlugin ITrayPluginInit)

public:
  Alarm();
  void Init(QSystemTrayIcon* tray_icon);

  void ExportSettings(QSettings::SettingsMap* settings);
  void ImportSettings(const QSettings::SettingsMap& settings);

public slots:
  void Start();
  void Stop();
  void Configure();
  void TimeUpdateListener();

private:
  QSystemTrayIcon* tray_icon_;
  QIcon old_icon_;
  PluginSettings* settings_;
  QPointer<QMediaPlayer> player_;
  bool icon_changed_;
};

} // namespace alarm

#endif // ALARM_H
