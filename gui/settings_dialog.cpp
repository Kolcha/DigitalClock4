#include "settings_dialog.h"
#include "ui_settings_dialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SettingsDialog)
{
  ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
  delete ui;
}

void SettingsDialog::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);
  switch (e->type()) {
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
      break;
    default:
      break;
  }
}
