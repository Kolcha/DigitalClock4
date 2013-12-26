#include "settings_keys.h"
#include "../power_off_settings.h"
#include "settings_dialog.h"
#include "ui_settings_dialog.h"

namespace power_off {

SettingsDialog::SettingsDialog(QWidget* parent)
  : QDialog(parent), ui(new Ui::SettingsDialog) {
  ui->setupUi(this);
  ui->time_edit->setDisplayFormat(GetDefaultValue(OPT_TIME_FORMAT).toString());
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

void SettingsDialog::SettingsListener(const QString& key, const QVariant& value) {
  if (key == OPT_TIME) ui->time_edit->setTime(value.value<QTime>());
}

void SettingsDialog::on_time_edit_timeChanged(const QTime& time) {
  emit OptionChanged(OPT_TIME, time);
}

} // namespace power_off
