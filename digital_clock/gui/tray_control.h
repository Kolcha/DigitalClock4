#ifndef DIGITAL_CLOCK_GUI_TRAY_CONTROL_H
#define DIGITAL_CLOCK_GUI_TRAY_CONTROL_H

#include <QObject>
#include <QSystemTrayIcon>

class QAction;

namespace digital_clock {
namespace gui {

class TrayControl : public QObject
{
  Q_OBJECT

public:
  explicit TrayControl(QWidget* parent = 0);
  QSystemTrayIcon* GetTrayIcon() const;
  QAction* GetShowHideAction() const;

signals:
  void VisibilityChanged(bool visible);
  void ShowSettingsDlg();
  void ShowAboutDlg();
  void CheckForUpdates();
  void AppExit();

private slots:
  void TrayEventHandler(QSystemTrayIcon::ActivationReason reason);
  void ShowHideHandler();

private:
  QSystemTrayIcon* tray_icon_;
  QAction* show_hide_action_;
};

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_TRAY_CONTROL_H
