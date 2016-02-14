#include "settings_keys.h"
#include "../power_off_settings.h"
#include "settings_dialog.h"
#include "ui_settings_dialog.h"

namespace power_off {

SettingsDialog::SettingsDialog(QWidget* parent)
  : QDialog(parent), ui(new Ui::SettingsDialog) {
  ui->setupUi(this);
  ui->time_edit->setDisplayFormat("hh:mm");
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

void SettingsDialog::SettingsListener(const QString& key, const QVariant& value) {
  if (key == OPT_TIME) ui->time_edit->setTime(value.value<QTime>());
  if (key == OPT_FORCE) ui->force_off->setChecked(value.toBool());
}

void SettingsDialog::on_time_edit_timeChanged(const QTime& time) {
  emit OptionChanged(OPT_TIME, time);
}

void SettingsDialog::on_force_off_toggled(bool checked) {
  emit OptionChanged(OPT_FORCE, checked);
}

} // namespace power_off
