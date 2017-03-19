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

#ifndef IP_ADDRES_SETTINGS_H
#define IP_ADDRES_SETTINGS_H

#include <QSettings>

namespace ip_address {

#define OPT_DISPLAY_INTERNAL_ADDRESSES    "plugins/ip_address/display_internal_addresses"
#define OPT_INTERNAL_INTERFACES_LIST      "plugins/ip_address/internal_interfaces_list"
#define OPT_DISPLAY_EXTERNAL_ADDRESS      "plugins/ip_address/display_external_address"

void InitDefaults(QSettings::SettingsMap* defaults);

} // namespace ip_address

#endif // IP_ADDRES_SETTINGS_H