/*
    Digital Clock: IP address plugin
    Copyright (C) 2016  Nick Korotysh <nick.korotysh@gmail.com>

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
#include <QVBoxLayout>
#include <QNetworkInterface>

#include "plugin_settings.h"

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
  // add widget with common settings configuration controls
  QVBoxLayout* dlg_layout = qobject_cast<QVBoxLayout*>(dialog->layout());
  Q_ASSERT(dlg_layout);
  dlg_layout->insertWidget(dlg_layout->count() - 1, InitConfigWidget(dialog));
  // connect main signals/slots
  connect(dialog, SIGNAL(accepted()), settings_, SLOT(Save()));
  connect(dialog, SIGNAL(rejected()), settings_, SLOT(Load()));
  dialog->show();
}

QWidget* IpAddressPlugin::InitWidget(QGridLayout* layout)
{
  msg_label_ = new QLabel();
  msg_label_->setAlignment(Qt::AlignCenter);
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

  foreach (const QNetworkInterface& iface, QNetworkInterface::allInterfaces()) {
    if (iface.flags() & QNetworkInterface::IsLoopBack) continue;
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
