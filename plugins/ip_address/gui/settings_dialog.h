/*
    Digital Clock: IP address plugin
    Copyright (C) 2016-2019  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef IP_ADDRESS_SETTINGS_DIALOG_H
#define IP_ADDRESS_SETTINGS_DIALOG_H

#include <QDialog>

#include <QSettings>

namespace ip_address {

class NetInterfacesModel;

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SettingsDialog(QWidget* parent = nullptr);
  ~SettingsDialog();

  void AddCommonWidget(QWidget* widget);

  void Init(const QSettings::SettingsMap& settings);

signals:
  void OptionChanged(const QString&, const QVariant&);

private slots:
  void onInterfacesListChanged(const QStringList& ilst);
  void on_show_internal_addr_cbox_clicked(bool checked);
  void on_show_external_addr_cbox_clicked(bool checked);

private:
  Ui::SettingsDialog* ui;
  NetInterfacesModel* iface_model_;
};


} // namespace ip_address

#endif // IP_ADDRESS_SETTINGS_DIALOG_H
