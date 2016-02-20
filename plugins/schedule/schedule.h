#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QSystemTrayIcon>
#include <QPointer>
#include "iclock_plugin.h"
#include "gui/settings_dialog.h"

class TaskManager;

namespace schedule {

class Schedule : public IClockPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "schedule.json")
  Q_INTERFACES(IClockPlugin)

public:
  Schedule();

public slots:
  void Start();
  void Stop();
  void Configure();
  void TimeUpdateListener();

private slots:
  void TrayActivated(QSystemTrayIcon::ActivationReason reason);
  void ShowMessage(const QString& message);

private:
  QPointer<QSystemTrayIcon> tray_icon_;
  QMenu* tray_menu_;
  TaskManager* manager_;
  QPointer<SettingsDialog> settings_dlg_;
};

} // namespace schedule

#endif // SCHEDULE_H
