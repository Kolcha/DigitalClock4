#ifndef TRAY_CONTROL_H
#define TRAY_CONTROL_H

#include <QMenu>
#include <QSystemTrayIcon>

namespace digital_clock {

class TrayControl : public QObject {
  Q_OBJECT
public:
  explicit TrayControl(QObject* parent = 0);
  QSystemTrayIcon* GetTrayIcon();

signals:
  void ShowSettingsDlg();
  void ShowAboutDlg();
  void CheckForUpdates();

private slots:
  void TrayEventHandler(QSystemTrayIcon::ActivationReason reason);

private:
  void CreateActions();
  void CreateTrayIcon();
  QAction* settings_action_;
  QAction* about_action_;
  QAction* update_action_;
  QAction* exit_action_;
  QSystemTrayIcon* tray_icon_;
};

} // namespace digital_clock

#endif // TRAY_CONTROL_H
