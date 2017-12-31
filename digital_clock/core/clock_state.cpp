/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2016-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "clock_state.h"

#include "settings_storage.h"

namespace digital_clock {
namespace core {

ClockState::ClockState(SettingsStorage* backend, QObject* parent) : SettingsStorageWrapper(backend, parent)
{
  is_exportable_ = true;
}

void ClockState::SetVariable(const QString& key, const QVariant& value, bool commit)
{
  if (GetVariable(key) == value) return;
  QString full_key = AddKeyPrefix(key);
  state_keys_.insert(full_key);
  SettingsStorageWrapper::setValue(full_key, value);
  if (commit) GetBackend()->Commit(full_key);
  if (!IsExportable()) GetBackend()->Forget(full_key);
}

QVariant ClockState::GetVariable(const QString& key, const QVariant& default_value)
{
  QString full_key = AddKeyPrefix(key);
  state_keys_.insert(full_key);
  QVariant value = SettingsStorageWrapper::getValue(full_key, default_value);
  if (!IsExportable()) GetBackend()->Forget(full_key);
  return value;
}

void ClockState::SetExportable(bool exportable)
{
  if (exportable) {
    for (auto& key : state_keys_) GetBackend()->GetValue(key);
  } else {
    for (auto& key : state_keys_) GetBackend()->Forget(key);
  }
  is_exportable_ = exportable;
}

bool ClockState::IsExportable() const
{
  return is_exportable_;
}

QString ClockState::AddKeyPrefix(const QString& key)
{
  return QString("%1/%2").arg("state", key);
}

} // namespace core
} // namespace digital_clock
