#ifndef MANAGER_H
#define MANAGER_H

#include <QIcon>
#include "iclock_plugin.h"

class Manager : public ITrayPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID TRAY_PLUGIN_INTERFACE_IID FILE "../alarm.json")
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

#endif // MANAGER_H
