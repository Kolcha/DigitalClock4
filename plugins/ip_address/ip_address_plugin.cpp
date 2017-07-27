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

#include <functional>

#include <QLabel>
#include <QTimer>
#include <QNetworkInterface>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "plugin_settings.h"

#include "core/ip_addres_settings.h"

#include "gui/settings_dialog.h"

namespace ip_address {

IpAddressPlugin::IpAddressPlugin() : msg_label_(nullptr), ip_update_timer_(nullptr), getting_external_ip_(false)
{
  InitTranslator(QLatin1String(":/ip_address/ip_address_"));
  info_.display_name = tr("IP address");
  info_.description = tr("Displays local IP address(es) under clock.");
  InitIcon(":/ip_address/icon.svg.p");
  plg_name_ = QString("ip_address");
  qnam_ = new QNetworkAccessManager(this);
}

void IpAddressPlugin::Start()
{
  ::plugin::WidgetPluginBase::Start();
  ip_update_timer_ = new QTimer();
  connect(ip_update_timer_, &QTimer::timeout, this, &IpAddressPlugin::UpdateIPsList);
  ip_update_timer_->setSingleShot(false);
  ip_update_timer_->start(15 * 60 * 1000);
}

void IpAddressPlugin::Stop()
{
  ::plugin::WidgetPluginBase::Stop();
  ip_update_timer_->stop();
  delete ip_update_timer_;
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
  // optimization: update IP addresses only by settings changes
  connect(dialog, SIGNAL(OptionChanged(QString,QVariant)), this, SLOT(UpdateIPsList()));
  connect(dialog, SIGNAL(accepted()), this, SLOT(UpdateIPsList()));
  connect(dialog, SIGNAL(rejected()), this, SLOT(UpdateIPsList()));
}

void IpAddressPlugin::InitSettingsDefaults(QSettings::SettingsMap* defaults)
{
  InitDefaults(defaults);
}

QWidget* IpAddressPlugin::InitWidget(QGridLayout* layout)
{
  msg_label_ = new QLabel();
  Q_UNUSED(layout);
  return msg_label_;
}

void IpAddressPlugin::DisplayImage(const QImage& image)
{
  msg_label_->setPixmap(QPixmap::fromImage(image));
}

QString IpAddressPlugin::GetWidgetText()
{
  // IP list is not initialized at first time
  if (last_ip_list_.isEmpty()) UpdateIPsList();
  return last_ip_list_;
}

void IpAddressPlugin::UpdateIPsList()
{
  last_ip_list_.clear();

  if (settings_->GetOption(OPT_DISPLAY_INTERNAL_ADDRESSES).toBool()) {
    // get interested internal addresses
    for (auto& iname : settings_->GetOption(OPT_INTERNAL_INTERFACES_LIST).toStringList()) {
      QNetworkInterface iface = QNetworkInterface::interfaceFromName(iname);
      if (!iface.isValid()) continue;
      if (iface.flags() & QNetworkInterface::IsUp) {
        foreach (const QNetworkAddressEntry& addren, iface.addressEntries()) {
          if (addren.ip().protocol() == QAbstractSocket::IPv4Protocol) {
            last_ip_list_ += addren.ip().toString() + '\n';
          }
        }
      }
    }

    if (last_ip_list_.isEmpty())
      last_ip_list_ = tr("<no interfaces found>");
  }

  if (settings_->GetOption(OPT_DISPLAY_EXTERNAL_ADDRESS).toBool() && !getting_external_ip_) {
    // get external IP address
    getting_external_ip_ = true;
    QNetworkReply* reply = qnam_->get(QNetworkRequest(QUrl("https://api.ipify.org/")));
    connect(reply, &QNetworkReply::finished, [=] () {
      if (!last_ip_list_.isEmpty() && *last_ip_list_.rbegin() != '\n')
        last_ip_list_ += '\n';
      getting_external_ip_ = false;
      if (reply->error() == QNetworkReply::NoError)
        last_ip_list_ += reply->readAll();
      else
        last_ip_list_ += reply->errorString();
      reply->deleteLater();
      TimeUpdateListener();
    });
  }

  if (!last_ip_list_.isEmpty() && *last_ip_list_.rbegin() == '\n')
    last_ip_list_.chop(1);
}

} // namespace ip_address
