/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2014-2017  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "plugin_list_item_widget.h"
#include "ui_plugin_list_item_widget.h"

#include "gui/plugin_info_dialog.h"

namespace digital_clock {
namespace gui {

PluginListItemWidget::PluginListItemWidget(QWidget* parent) :
  QWidget(parent),
  ui(new Ui::PluginListItemWidget)
{
  ui->setupUi(this);
}

PluginListItemWidget::~PluginListItemWidget()
{
  delete ui;
}

QString PluginListItemWidget::GetName() const
{
  return info_.metadata[PI_NAME];
}

void PluginListItemWidget::SetDisplayName(const QString& name)
{
  ui->name_check_box->setText(name);
}

void PluginListItemWidget::SetVersion(const QString& version)
{
  ui->version_label->setText(version);
}

void PluginListItemWidget::SetConfigurable(bool configable)
{
  ui->config_btn->setEnabled(configable);
}

void PluginListItemWidget::SetChecked(bool checked)
{
  ui->name_check_box->setChecked(checked);
}

void PluginListItemWidget::SetInfo(const TPluginInfo& info)
{
  info_ = info;
  SetDisplayName(info.gui_info.display_name);
  SetVersion(info.metadata[PI_VERSION]);
}

void PluginListItemWidget::on_name_check_box_toggled(bool checked)
{
  emit StateChanged(GetName(), checked);
}

void PluginListItemWidget::on_config_btn_clicked()
{
  emit ConfigureRequested(GetName());
}

void PluginListItemWidget::on_info_btn_clicked()
{
  PluginInfoDialog dialog(window());
  dialog.SetInfo(info_);
  dialog.exec();
}

} // namespace gui
} // namespace digital_clock
