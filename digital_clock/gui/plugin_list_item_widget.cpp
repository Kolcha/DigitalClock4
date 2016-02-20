#include "plugin_list_item_widget.h"
#include "ui_plugin_list_item_widget.h"

#include "gui/plugin_info_dialog.h"

namespace digital_clock {
namespace gui {

PluginListItemWidget::PluginListItemWidget(QWidget* parent)
  : QWidget(parent), ui(new Ui::PluginListItemWidget) {
  ui->setupUi(this);
}

PluginListItemWidget::~PluginListItemWidget() {
  delete ui;
}

QString PluginListItemWidget::GetName() const {
  return info_.metadata[PI_NAME];
}

void PluginListItemWidget::SetDisplayName(const QString& name) {
  ui->name_check_box->setText(name);
}

void PluginListItemWidget::SetVersion(const QString& version) {
  ui->version_label->setText(version);
}

void PluginListItemWidget::SetConfigurable(bool configable) {
  ui->config_btn->setEnabled(configable);
}

void PluginListItemWidget::SetChecked(bool checked) {
  ui->name_check_box->setChecked(checked);
}

void PluginListItemWidget::SetInfo(const TPluginInfo& info) {
  info_ = info;
  SetDisplayName(info.gui_info.display_name);
  SetVersion(info.metadata[PI_VERSION]);
}

void PluginListItemWidget::on_name_check_box_toggled(bool checked) {
  emit StateChanged(GetName(), checked);
}

void PluginListItemWidget::on_config_btn_clicked() {
  emit ConfigureRequested(GetName());
}

void PluginListItemWidget::on_info_btn_clicked() {
  PluginInfoDialog* dialog = new PluginInfoDialog(this);
  dialog->SetInfo(info_);
  dialog->show();
}

} // namespace gui
} // namespace digital_clock
