#ifndef TRAY_CONTROL_H
#define TRAY_CONTROL_H

#include <QMenu>
#include <QSystemTrayIcon>

class TrayControl : public QObject {
  Q_OBJECT
public:
  explicit TrayControl(QObject* parent = 0);
  QSystemTrayIcon* GetTrayIcon();

signals:
  void ShowSettingsDlg();
  void ShowAboutDlg();

private:
  void CreateActions();
  void CreateTrayIcon();
  QAction* settings_action_;
  QAction* about_action_;
  QAction* exit_action_;
  QSystemTrayIcon* tray_icon_;
};

#endif // TRAY_CONTROL_H
