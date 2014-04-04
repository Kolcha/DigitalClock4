#ifndef DIGITAL_CLOCK_GUI_TRAY_CONTROL_H
#define DIGITAL_CLOCK_GUI_TRAY_CONTROL_H

#include <QSystemTrayIcon>

namespace digital_clock {
namespace gui {

class TrayControl : public QObject {
  Q_OBJECT

public:
  explicit TrayControl(QWidget* parent = 0);
  QSystemTrayIcon* GetTrayIcon();

signals:
  void ShowSettingsDlg();
  void ShowAboutDlg();
  void CheckForUpdates();

public slots:

private slots:
  void TrayEventHandler(QSystemTrayIcon::ActivationReason reason);

private:
  QSystemTrayIcon* tray_icon_;
};

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_TRAY_CONTROL_H
