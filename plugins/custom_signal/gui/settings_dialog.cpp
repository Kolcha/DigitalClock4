#include "settings_dialog.h"
#include "ui_settings_dialog.h"

namespace custom_signal {

SettingsDialog::SettingsDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::SettingsDialog)
{
  ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
  delete ui;
}

} // namespace custom_signal
