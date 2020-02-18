/*
    Digital Clock: clock common library
    Copyright (C) 2016-2020  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef SETTINGS_STORAGE_WRAPPER_H
#define SETTINGS_STORAGE_WRAPPER_H

#include <QObject>

#include <QString>
#include <QVariant>
#include <QSet>

#include "clock_common_global.h"

class SettingsStorage;

class CLOCK_COMMON_EXPORT SettingsStorageWrapper : public QObject
{
  Q_OBJECT
public:
  explicit SettingsStorageWrapper(SettingsStorage* backend, QObject* parent = nullptr);

  /*!
   * Get storage backend.
   * @return pointer to SettingsStorage object
   */
  SettingsStorage* GetBackend() const { return backend_; }

signals:
  void accepted();
  void rejected();

public slots:
  void Accept();
  void Reject();

protected:
  void setValue(const QString& key, const QVariant& value);
  QVariant getValue(const QString& key, const QVariant& default_value = QVariant()) const;
  void remove(const QString& key);

private:
  SettingsStorage* backend_;
  QSet<QString> changed_keys_;
};

#endif // SETTINGS_STORAGE_WRAPPER_H
