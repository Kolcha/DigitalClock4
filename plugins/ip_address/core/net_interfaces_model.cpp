/*
    Digital Clock: IP address plugin
    Copyright (C) 2017-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "net_interfaces_model.h"

namespace ip_address {

NetInterfacesModel::NetInterfacesModel(QObject* parent)
  : QAbstractListModel(parent)
{
  interfaces_ = QNetworkInterface::allInterfaces();
}

int NetInterfacesModel::rowCount(const QModelIndex& parent) const
{
  if (parent.isValid()) return 0;
  return interfaces_.size();
}

QVariant NetInterfacesModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid()) return QVariant();

  const QNetworkInterface& iface = interfaces_.at(index.row());

  switch (role) {
    case Qt::DisplayRole:
      return iface.humanReadableName();

    case Qt::CheckStateRole:
      return checked_interfaces_.contains(iface.name()) ? Qt::Checked : Qt::Unchecked;
  }

  return QVariant();
}

bool NetInterfacesModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if (data(index, role) != value) {
    if (role == Qt::CheckStateRole) {
      const QNetworkInterface& iface = interfaces_[index.row()];
      if (value.value<Qt::CheckState>() == Qt::Checked)
        checked_interfaces_.insert(iface.name());
      else
        checked_interfaces_.remove(iface.name());
      emit checkedInterfacesChanged(getCheckedInterfaces());
    }
    emit dataChanged(index, index, QVector<int>() << role);
    return true;
  }
  return false;
}

Qt::ItemFlags NetInterfacesModel::flags(const QModelIndex& index) const
{
  if (!index.isValid()) return Qt::NoItemFlags;

  Qt::ItemFlags flags = Qt::ItemIsUserCheckable;
  if (interfaces_[index.row()].flags() & QNetworkInterface::IsUp) flags |= Qt::ItemIsEnabled;

  return flags;
}

QStringList NetInterfacesModel::getCheckedInterfaces() const
{
  return checked_interfaces_.toList();
}

void NetInterfacesModel::setCheckedInterfaces(const QStringList& ifaces_list)
{
  checked_interfaces_ = QSet<QString>::fromList(ifaces_list);

  for (auto iter = checked_interfaces_.begin(); iter != checked_interfaces_.end();) {
    bool exists = false;
    for (auto& iface : qAsConst(interfaces_)) {
      if (*iter == iface.name()) {
        exists = true;
        break;
      }
    }

    if (!exists)
      checked_interfaces_.erase(iter++);
    else
      ++iter;
  }

  if (checked_interfaces_.size() != ifaces_list.size()) emit checkedInterfacesChanged(getCheckedInterfaces());
}

} // namespace ip_address
