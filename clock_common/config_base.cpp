/*
    Digital Clock: clock common library
    Copyright (C) 2013-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "config_base.h"

ConfigBase::ConfigBase(SettingsStorage* backend, QObject* parent) : SettingsStorageWrapper(backend, parent)
{
}

QVariant ConfigBase::GetValue(const int id) const
{
  return SettingsStorageWrapper::getValue(GetKey(id), GetDefaultValue(id));
}

void ConfigBase::SetValue(const int id, const QVariant& value)
{
  SettingsStorageWrapper::setValue(GetKey(id), value);
}
