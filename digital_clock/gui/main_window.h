#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QPointer>
#include "../core/clock_settings.h"
#include "../core/plugin_manager.h"
#include "../core/skin_manager.h"
#include "../core/updater.h"
#include "skin_drawer.h"
#include "digital_clock.h"
#include "tray_control.h"

class SettingsDialog;
class AboutDialog;

class MainWindow : public QWidget {
  Q_OBJECT
public:
  explicit MainWindow(QWidget* parent = 0);
  void Init();

protected:
  void mouseMoveEvent(QMouseEvent* event);
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);
  void showEvent(QShowEvent* event);

private slots:
  void SettingsListener(Options opt, const QVariant& value);
  void ShowSettingsDialog();
  void EndSettingsEdit();
  void ShowAboutDialog();
  void DisplayMenu(const QPoint& pos);

private:
  void ConnectAll();
  void SetWindowFlag(Qt::WindowFlags flag, bool set);

  ClockSettings* settings_;
  PluginManager* plugin_manager_;
  SkinManager* skin_manager_;
  SkinDrawer* drawer_;
  DigitalClock* d_clock_;
  TrayControl* tray_control_;
  QPoint drag_position_;
  QStringList active_plugins_;
  QPointer<SettingsDialog> settings_dlg_;
  QPointer<AboutDialog> about_dlg_;
  Updater* updater_;
};

#endif // MAIN_WINDOW_H
