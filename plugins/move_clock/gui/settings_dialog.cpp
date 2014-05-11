#include "settings_dialog.h"
#include "../move_clock_settings.h"
#include "ui_settings_dialog.h"

namespace move_clock {

SettingsDialog::SettingsDialog(QWidget* parent)
  : QDialog(parent), ui(new Ui::SettingsDialog) {
  ui->setupUi(this);
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

void SettingsDialog::SettingsListener(const QString& key, const QVariant& value) {
  if (key == OPT_X0) ui->x0_value->setValue(value.toReal());
  if (key == OPT_KX) ui->kx_value->setValue(value.toReal());
  if (key == OPT_AX) ui->ax_value->setValue(value.toReal());
  if (key == OPT_NX) ui->nx_value->setValue(value.toReal());
  if (key == OPT_Y0) ui->y0_value->setValue(value.toReal());
  if (key == OPT_KY) ui->ky_value->setValue(value.toReal());
  if (key == OPT_AY) ui->ay_value->setValue(value.toReal());
  if (key == OPT_NY) ui->ny_value->setValue(value.toReal());
  if (key == OPT_TIMEOUT) ui->speed_slider->setValue(value.toInt());
}

} // namespace move_clock

void move_clock::SettingsDialog::on_x0_value_valueChanged(double arg1) {
  emit OptionChanged(OPT_X0, arg1);
}

void move_clock::SettingsDialog::on_kx_value_valueChanged(double arg1) {
  emit OptionChanged(OPT_KX, arg1);
}

void move_clock::SettingsDialog::on_ax_value_valueChanged(double arg1) {
  emit OptionChanged(OPT_AX, arg1);
}

void move_clock::SettingsDialog::on_nx_value_valueChanged(double arg1) {
  emit OptionChanged(OPT_NX, arg1);
}

void move_clock::SettingsDialog::on_y0_value_valueChanged(double arg1) {
  emit OptionChanged(OPT_Y0, arg1);
}

void move_clock::SettingsDialog::on_ky_value_valueChanged(double arg1) {
  emit OptionChanged(OPT_KY, arg1);
}

void move_clock::SettingsDialog::on_ay_value_valueChanged(double arg1) {
  emit OptionChanged(OPT_AY, arg1);
}

void move_clock::SettingsDialog::on_ny_value_valueChanged(double arg1) {
  emit OptionChanged(OPT_NY, arg1);
}

void move_clock::SettingsDialog::on_speed_slider_valueChanged(int value) {
  emit OptionChanged(OPT_TIMEOUT, value);
}
