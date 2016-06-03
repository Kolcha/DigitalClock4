#ifndef SCHEDULE_SCHEDULE_H
#define SCHEDULE_SCHEDULE_H

#include "iclock_plugin.h"

#include <QPointer>
#include <QSystemTrayIcon>

#include "core/task.h"

namespace schedule {

class TasksStorage;
class TasksInvoker;

class Schedule : public IClockPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "schedule.json")
  Q_INTERFACES(IClockPlugin)

public:
  Schedule();

  void InitSettings(SettingsStorage* backend) override;

public slots:
  void Start()     override;
  void Stop()      override;

  void Configure() override;

private slots:
  void TrayActivated(QSystemTrayIcon::ActivationReason reason);
  void TaskCompleted(const TaskPtr& task);

private:
  QPointer<QSystemTrayIcon> tray_icon_;
  QMenu* tray_menu_;

  TasksStorage* backend_;
  TasksInvoker* invoker_;
};

} // namespace schedule

#endif // SCHEDULE_SCHEDULE_H
