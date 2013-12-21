#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QSystemTrayIcon>
#include <QPointer>
#include "iclock_plugin.h"
#include "gui/settings_dlg.h"

class QTranslator;
class TaskManager;
class PluginSettings;

class Schedule : public IClockPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "schedule.json")
  Q_INTERFACES(IClockPlugin)

public:
  Schedule();
  ~Schedule();

public slots:
  void Start();
  void Stop();
  void Configure();
  void SettingsListener(Options, const QVariant&) {}
  void TimeUpdateListener();

private slots:
  void TrayActivated(QSystemTrayIcon::ActivationReason reason);
  void ShowMessage(const QString& message);

private:
  QTranslator* translator_;
  QPointer<QSystemTrayIcon> tray_icon_;
  QMenu* tray_menu_;
  TaskManager* manager_;
  QPointer<SettingsDlg> settings_dlg_;
  PluginSettings* settings_;
};

#endif // SCHEDULE_H
