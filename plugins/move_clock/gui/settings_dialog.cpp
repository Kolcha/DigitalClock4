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
