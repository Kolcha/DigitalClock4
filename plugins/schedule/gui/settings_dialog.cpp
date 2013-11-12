#include "add_task_dialog.h"
#include "settings_dialog.h"
#include "ui_settings_dialog.h"

SettingsDialog::SettingsDialog(QWidget* parent)
  : CenteredDialog(parent), ui(new Ui::SettingsDialog) {
  ui->setupUi(this);
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

void SettingsDialog::on_add_btn_clicked() {
  AddTaskDialog* dlg = new AddTaskDialog(this);
  dlg->show();
}
