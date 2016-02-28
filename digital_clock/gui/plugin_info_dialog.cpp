#include "plugin_info_dialog.h"
#include "ui_plugin_info_dialog.h"

namespace digital_clock {
namespace gui {

PluginInfoDialog::PluginInfoDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::PluginInfoDialog)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);
  setWindowIcon(QIcon(":/clock/images/about.svg"));
}

PluginInfoDialog::~PluginInfoDialog() {
  delete ui;
}

void PluginInfoDialog::SetInfo(const TPluginInfo& info) {
  ui->name_value->setText(info.gui_info.display_name);
  ui->version_value->setText(tr("version: %1").arg(info.metadata[PI_VERSION]));
  if (info.gui_info.icon.isNull()) ui->icon_label->hide();
  else ui->icon_label->setPixmap(info.gui_info.icon);
  ui->description_value->setText(info.gui_info.description);
  ui->author_value->setText(info.metadata[PI_AUTHOR]);
  ui->email_value->setText(info.metadata[PI_EMAIL]);
  adjustSize();
  setMaximumSize(sizeHint());
}

} // namespace gui
} // namespace digital_clock
