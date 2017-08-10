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

#ifndef IP_ADDRESS_NET_INTERFACES_MODEL_H
#define IP_ADDRESS_NET_INTERFACES_MODEL_H

#include <QAbstractListModel>

#include <QNetworkInterface>
#include <QList>
#include <QSet>

namespace ip_address {

class NetInterfacesModel : public QAbstractListModel
{
  Q_OBJECT

  Q_PROPERTY(QStringList checkedInterfaces READ getCheckedInterfaces WRITE setCheckedInterfaces NOTIFY checkedInterfacesChanged)

public:
  explicit NetInterfacesModel(QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  bool setData(const QModelIndex& index, const QVariant& value,
               int role = Qt::EditRole) override;

  Qt::ItemFlags flags(const QModelIndex& index) const override;

  QStringList getCheckedInterfaces() const;

signals:
  void checkedInterfacesChanged(const QStringList& ifaces_list);

public slots:
  void setCheckedInterfaces(const QStringList& ifaces_list);

private:
  QList<QNetworkInterface> interfaces_;
  QSet<QString> checked_interfaces_;
};

} // namespace ip_address

#endif // IP_ADDRESS_NET_INTERFACES_MODEL_H
