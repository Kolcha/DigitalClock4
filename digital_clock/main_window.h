#ifndef DIGITAL_CLOCK_MAIN_WINDOW_H
#define DIGITAL_CLOCK_MAIN_WINDOW_H

#include <QWidget>

#include <QTimer>

#include "settings_keys.h"

class SettingsStorage;


namespace digital_clock {

namespace core {
class ClockSettings;
class SkinManager;
class Updater;
}

namespace gui {
class TrayControl;
class ClockWidget;
}


class MainWindow : public QWidget
{
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

signals:

public slots:

protected:
  void mousePressEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);
  void paintEvent(QPaintEvent* event);

private slots:
  void Reset();
  void ApplyOption(const Option opt, const QVariant& value);

  void ShowSettingsDialog();
  void ShowAboutDialog();

  void Update();

private:
  void SetWindowFlag(Qt::WindowFlags flag, bool set);

  SettingsStorage* config_backend_;

  core::ClockSettings* app_config_;
  core::SkinManager* skin_manager_;
  core::Updater* updater_;

  gui::TrayControl* tray_control_;
  gui::ClockWidget* clock_widget_;

  QTimer timer_;

  QPoint drag_position_;
  CAlignment cur_alignment_;
};

} // namespace digital_clock

#endif // DIGITAL_CLOCK_MAIN_WINDOW_H
