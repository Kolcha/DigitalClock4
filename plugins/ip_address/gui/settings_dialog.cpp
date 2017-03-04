/*
    Digital Clock: IP address plugin
    Copyright (C) 2016-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "settings_dialog.h"
#include "ui_settings_dialog.h"

#include "core/ip_addres_settings.h"
#include "core/net_interfaces_model.h"

namespace ip_address {

SettingsDialog::SettingsDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::SettingsDialog)
{
  setAttribute(Qt::WA_DeleteOnClose);
  ui->setupUi(this);
  iface_model_ = new NetInterfacesModel(this);
  ui->interfaces_list->setModel(iface_model_);
  connect(iface_model_, &NetInterfacesModel::checkedInterfacesChanged, this, &SettingsDialog::onInterfacesListChanged);
}

SettingsDialog::~SettingsDialog()
{
  delete ui;
}

void SettingsDialog::AddCommonWidget(QWidget* widget)
{
  widget->layout()->setMargin(12);
  ui->tabWidget->addTab(widget, tr("Appearance"));
}

void SettingsDialog::Init(const QSettings::SettingsMap& settings)
{
  ui->show_internal_addr_cbox->setChecked(settings.value(OPT_DISPLAY_INTERNAL_ADDRESSES).toBool());
  iface_model_->setCheckedInterfaces(settings.value(OPT_INTERNAL_INTERFACES_LIST).toStringList());
  ui->show_external_addr_cbox->setChecked(settings.value(OPT_DISPLAY_EXTERNAL_ADDRESS).toBool());
}

void SettingsDialog::onInterfacesListChanged(const QStringList& ilst)
{
  emit OptionChanged(OPT_INTERNAL_INTERFACES_LIST, ilst);
}

void SettingsDialog::on_show_internal_addr_cbox_clicked(bool checked)
{
  emit OptionChanged(OPT_DISPLAY_INTERNAL_ADDRESSES, checked);
}

void SettingsDialog::on_show_external_addr_cbox_clicked(bool checked)
{
  emit OptionChanged(OPT_DISPLAY_EXTERNAL_ADDRESS, checked);
}

} // namespace ip_address
