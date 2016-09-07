#include "settings_dialog.h"
#include "ui_settings_dialog.h"

#include "quick_note_settings.h"

namespace quick_note {

SettingsDialog::SettingsDialog(QWidget* parent) :
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

void SettingsDialog::Init(const QSettings::SettingsMap& settings)
{
  this->blockSignals(true);
  for (auto iter = settings.constBegin(); iter != settings.constEnd(); ++iter) {
    if (iter.key() == OPT_QUICK_NOTE_MSG) {
      ui->note_edit->setText(iter.value().toString());
    }
    if (iter.key() == OPT_SHOW_EDIT_BTN) {
      ui->edit_btn_enabled->setChecked(iter.value().toBool());
    }
  }
  this->blockSignals(false);
}

void SettingsDialog::on_note_edit_textEdited(const QString& arg1)
{
  emit OptionChanged(OPT_QUICK_NOTE_MSG, arg1);
}

void SettingsDialog::on_edit_btn_enabled_clicked(bool checked)
{
  emit OptionChanged(OPT_SHOW_EDIT_BTN, checked);
}

} // namespace quick_note
