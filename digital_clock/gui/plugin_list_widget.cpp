#include "plugin_info_dialog.h"
#include "plugin_list_widget.h"
#include "ui_plugin_list_widget.h"

namespace digital_clock {

PluginListWidget::PluginListWidget(QWidget* parent)
  : QWidget(parent), ui(new Ui::PluginListWidget) {
  ui->setupUi(this);
}

PluginListWidget::~PluginListWidget() {
  delete ui;
}

QString PluginListWidget::GetName() const {
  return info_.metadata[PI_NAME];
}

void PluginListWidget::SetDisplayName(const QString& name) {
  ui->name_check_box->setText(name);
}

void PluginListWidget::SetVersion(const QString& version) {
  ui->version_label->setText(version);
}

void PluginListWidget::SetConfigurable(bool configable) {
  ui->config_btn->setEnabled(configable);
}

void PluginListWidget::SetChecked(bool checked) {
  ui->name_check_box->setChecked(checked);
}

void PluginListWidget::SetInfo(const TPluginInfo& info) {
  info_ = info;
  SetDisplayName(info.gui_info.display_name);
  SetVersion(info.metadata[PI_VERSION]);
}

void PluginListWidget::on_name_check_box_toggled(bool checked) {
  emit StateChanged(GetName(), checked);
}

void PluginListWidget::on_config_btn_clicked() {
  emit ConfigureRequested(GetName());
}

void PluginListWidget::on_info_btn_clicked() {
  PluginInfoDialog* dialog = new PluginInfoDialog(this);
  dialog->SetInfo(info_);
  dialog->show();
}

} // namespace digital_clock
