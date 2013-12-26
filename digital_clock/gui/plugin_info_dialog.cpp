#include "plugin_info_dialog.h"
#include "ui_plugin_info_dialog.h"

namespace digital_clock {

PluginInfoDialog::PluginInfoDialog(QWidget* parent)
  : CenteredDialog(parent), ui(new Ui::PluginInfoDialog) {
  ui->setupUi(this);
}

PluginInfoDialog::~PluginInfoDialog() {
  delete ui;
}

void PluginInfoDialog::SetInfo(const TPluginInfo& info) {
  ui->name_value->setText(info.gui_info.display_name);
  ui->version_value->setText(info.metadata[PI_VERSION]);
  ui->description_value->setText(info.gui_info.description);
  ui->author_value->setText(info.metadata[PI_AUTHOR]);
  ui->email_value->setText(info.metadata[PI_EMAIL]);
  adjustSize();
}

} // namespace digital_clock
