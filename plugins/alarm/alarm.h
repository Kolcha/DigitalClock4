#ifndef ALARM_H
#define ALARM_H

#include <QIcon>
#include <QPointer>
#include <QMediaPlayer>
#include "iclock_plugin.h"
#include "plugin_settings.h"

class SettingsDlg;

class Alarm : public ITrayPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID TRAY_PLUGIN_INTERFACE_IID FILE "alarm.json")
  Q_INTERFACES(IClockPlugin ITrayPlugin)

public:
  Alarm();
  void Init(QSystemTrayIcon* tray_icon, QWidget* parent);
  void GetInfo(TPluginInfo* info);

public slots:
  void Start();
  void Stop();
  void Configure();
  void SettingsListener(Options, const QVariant&) {}
  void TimeUpdateListener(const QString&);

private:
  QSystemTrayIcon* tray_icon_;
  QWidget* parent_;
  QIcon old_icon_;
  PluginSettings* settings_;
  QPointer<QMediaPlayer> player_;
  QPointer<SettingsDlg> dialog_;
  bool icon_changed_;
};

#endif // ALARM_H
