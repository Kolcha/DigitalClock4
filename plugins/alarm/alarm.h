#ifndef ALARM_H
#define ALARM_H

#include <QIcon>
#include <QPointer>
#include <QMediaPlayer>
#include "iclock_plugin.h"

class QTranslator;
class PluginSettings;

namespace alarm {

class Alarm : public ITrayPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID TRAY_PLUGIN_INTERFACE_IID FILE "alarm.json")
  Q_INTERFACES(IClockPlugin ITrayPlugin)

public:
  Alarm();
  ~Alarm();
  void Init(QSystemTrayIcon* tray_icon);

public slots:
  void Start();
  void Stop();
  void Configure();
  void SettingsListener(Options, const QVariant&) {}
  void TimeUpdateListener();

private:
  QTranslator* translator_;
  QSystemTrayIcon* tray_icon_;
  QIcon old_icon_;
  PluginSettings* settings_;
  QPointer<QMediaPlayer> player_;
  bool icon_changed_;
};

} // namespace alarm

#endif // ALARM_H
