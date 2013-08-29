#ifndef ALARM_H
#define ALARM_H

#include <QIcon>
#include "iclock_plugin.h"

class Alarm : public ITrayPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID TRAY_PLUGIN_INTERFACE_IID FILE "alarm.json")
  Q_INTERFACES(IClockPlugin ITrayPlugin)

public:
  void Init(QSystemTrayIcon* tray_icon);
  void Configure();
  void Start();
  void Stop();
  void GetInfo(TPluginInfo* info);

public slots:
  void SettingsListener(Options, const QVariant&) {}
  void TimeUpdateListener(const QString&);

private:
  QSystemTrayIcon* tray_icon_;
  QIcon old_icon_;
};

#endif // ALARM_H
