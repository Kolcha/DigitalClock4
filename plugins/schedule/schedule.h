#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QSystemTrayIcon>
#include <QPointer>
#include "iclock_plugin.h"
#include "gui/settings_dialog.h"

class TaskManager;

class Schedule : public IWidgetPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID WIDGET_PLUGIN_INTERFACE_IID FILE "schedule.json")
  Q_INTERFACES(IClockPlugin IWidgetPlugin)

public:
  Schedule();
  void Init(QWidget* main_wnd);

public slots:
  void Start();
  void Stop();
  void Configure();
  void SettingsListener(Options, const QVariant&) {}
  void TimeUpdateListener(const QString&);

private slots:
  void TrayActivated(QSystemTrayIcon::ActivationReason reason);
  void ShowMessage(const QString& message);

private:
  QWidget* parent_;
  QPointer<QSystemTrayIcon> tray_icon_;
  QMenu* tray_menu_;
  TaskManager* manager_;
  QPointer<SettingsDialog> settings_dlg_;
};

#endif // SCHEDULE_H
