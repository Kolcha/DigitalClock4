/*
    Digital Clock: IP address plugin
    Copyright (C) 2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "ip_addres_settings.h"

#include <QNetworkInterface>

namespace ip_address {

void InitDefaults(QSettings::SettingsMap* defaults)
{
  QStringList def_iface;
  foreach (const QNetworkInterface& iface, QNetworkInterface::allInterfaces()) {
    if (iface.flags() & QNetworkInterface::IsLoopBack) continue;
    if (iface.flags() & QNetworkInterface::IsUp) {
      def_iface.append(iface.name());
      break;
    }
  }

  defaults->insert(OPT_DISPLAY_INTERNAL_ADDRESSES, true);
  defaults->insert(OPT_INTERNAL_INTERFACES_LIST, def_iface);
  defaults->insert(OPT_DISPLAY_EXTERNAL_ADDRESS, false);
}

} // namespace ip_address
