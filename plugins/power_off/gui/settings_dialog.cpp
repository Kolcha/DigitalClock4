#include "settings_dialog.h"
#include "ui_settings_dialog.h"

namespace power_off {

SettingsDialog::SettingsDialog(QWidget* parent)
  : QDialog(parent), ui(new Ui::SettingsDialog) {
  ui->setupUi(this);
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

} // namespace power_off
