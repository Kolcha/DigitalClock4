#include "settings_dlg.h"
#include "ui_settings_dlg.h"

SettingsDlg::SettingsDlg(QWidget* parent)
  : QDialog(parent), ui(new Ui::SettingsDlg) {
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowIcon(QIcon(":/settings.svg"));
}

SettingsDlg::~SettingsDlg() {
  delete ui;
}
