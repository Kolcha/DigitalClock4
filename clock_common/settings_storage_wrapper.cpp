/*
    Digital Clock: clock common library
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

#include "settings_storage_wrapper.h"

#include "settings_storage.h"

SettingsStorageWrapper::SettingsStorageWrapper(SettingsStorage* backend, QObject* parent) :
  QObject(parent),
  backend_(backend)
{
  connect(backend_, &SettingsStorage::reloaded, this, &SettingsStorageWrapper::Reject);
}

void SettingsStorageWrapper::Accept()
{
  for (auto& key : changed_keys_) {
    backend_->Commit(key);
  }
  changed_keys_.clear();
  emit accepted();
}

void SettingsStorageWrapper::Reject()
{
  for (auto& key : changed_keys_) {
    backend_->Revert(key);
  }
  changed_keys_.clear();
  emit rejected();
}

void SettingsStorageWrapper::setValue(const QString& key, const QVariant& value)
{
  changed_keys_.insert(key);
  backend_->SetValue(key, value);
}

QVariant SettingsStorageWrapper::getValue(const QString& key, const QVariant& default_value) const
{
  return backend_->GetValue(key, default_value);
}

void SettingsStorageWrapper::remove(const QString& key)
{
  changed_keys_.insert(key);
  backend_->Remove(key);
}
