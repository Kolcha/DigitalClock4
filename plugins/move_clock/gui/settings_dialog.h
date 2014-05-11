#ifndef MOVE_CLOCK_SETTINGS_DIALOG_H
#define MOVE_CLOCK_SETTINGS_DIALOG_H

#include <QDialog>

namespace move_clock {

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
  Q_OBJECT

public:
  explicit SettingsDialog(QWidget* parent = 0);
  ~SettingsDialog();

public slots:
  void SettingsListener(const QString& key, const QVariant& value);

signals:
  void OptionChanged(const QString&, const QVariant&);

private slots:
  void on_x0_value_valueChanged(double arg1);
  void on_kx_value_valueChanged(double arg1);
  void on_ax_value_valueChanged(double arg1);
  void on_nx_value_valueChanged(double arg1);
  void on_y0_value_valueChanged(double arg1);
  void on_ky_value_valueChanged(double arg1);
  void on_ay_value_valueChanged(double arg1);
  void on_ny_value_valueChanged(double arg1);
  void on_speed_slider_valueChanged(int value);

private:
  Ui::SettingsDialog* ui;
};


} // namespace move_clock

#endif // MOVE_CLOCK_SETTINGS_DIALOG_H
