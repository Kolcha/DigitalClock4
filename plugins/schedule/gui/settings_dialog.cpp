#include "settings_dialog.h"
#include "ui_settings_dialog.h"

SettingsDialog::SettingsDialog(QWidget* parent)
  : CenteredDialog(parent), ui(new Ui::SettingsDialog) {
  ui->setupUi(this);
  setWindowIcon(":/settings.svg");
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}
