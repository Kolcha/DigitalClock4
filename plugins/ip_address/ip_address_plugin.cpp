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

#include "ip_address_plugin.h"

#include <QLabel>
#include <QGridLayout>
#include <QNetworkInterface>

#include "plugin_settings.h"

#include "core/ip_addres_settings.h"

#include "gui/settings_dialog.h"

namespace ip_address {

IpAddressPlugin::IpAddressPlugin() : msg_label_(nullptr)
{
  InitTranslator(QLatin1String(":/ip_address/ip_address_"));
  info_.display_name = tr("IP address");
  info_.description = tr("Displays local IP address(es) under clock.");
  InitIcon(":/ip_address/icon.svg");
  plg_name_ = QString("ip_address");
}

void IpAddressPlugin::Configure()
{
  SettingsDialog* dialog = new SettingsDialog();
  // load current settings to dialog
  QSettings::SettingsMap curr_settings;
  InitDefaults(&curr_settings);
  for (auto iter = curr_settings.begin(); iter != curr_settings.end(); ++iter) {
    *iter = settings_->GetOption(iter.key());
  }
  dialog->Init(curr_settings);
  // add widget with common settings configuration controls
  dialog->AddCommonWidget(InitConfigWidget(dialog));
  // connect main signals/slots
  connect(dialog, SIGNAL(OptionChanged(QString,QVariant)),
          settings_, SLOT(SetOption(QString,QVariant)));
  connect(dialog, SIGNAL(accepted()), settings_, SLOT(Save()));
  connect(dialog, SIGNAL(rejected()), settings_, SLOT(Load()));
  dialog->show();
}

void IpAddressPlugin::InitSettingsDefaults(QSettings::SettingsMap* defaults)
{
  InitDefaults(defaults);
}

QWidget* IpAddressPlugin::InitWidget(QGridLayout* layout)
{
  msg_label_ = new QLabel();
  layout->addWidget(msg_label_, layout->rowCount(), 0, 1, layout->columnCount());
  return msg_label_;
}

void IpAddressPlugin::DisplayImage(const QImage& image)
{
  msg_label_->setPixmap(QPixmap::fromImage(image));
}

QString IpAddressPlugin::GetWidgetText()
{
  QString ip_list;

  for (auto& iname : settings_->GetOption(OPT_INTERNAL_INTERFACES_LIST).toStringList()) {
    QNetworkInterface iface = QNetworkInterface::interfaceFromName(iname);
    if (!iface.isValid()) continue;
    if (iface.flags() & QNetworkInterface::IsUp) {
      foreach (const QNetworkAddressEntry& addren, iface.addressEntries()) {
        if (addren.ip().protocol() == QAbstractSocket::IPv4Protocol) {
          ip_list += addren.ip().toString() + '\n';
        }
      }
    }
  }

  if (!ip_list.isEmpty()) {
    ip_list.chop(1);
  } else {
    ip_list = tr("<no interfaces found>");
  }

  return ip_list;
}

} // namespace ip_address
