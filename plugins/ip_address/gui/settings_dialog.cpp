#include "settings_dialog.h"
#include "ui_settings_dialog.h"

namespace ip_address {

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

} // namespace ip_address
