/*
    Digital Clock: clock common library
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

#ifndef CONFIG_BASE_H
#define CONFIG_BASE_H

#include "settings_storage_wrapper.h"

#include "clock_common_global.h"

/*!
 * @brief Base class for configs.
 *
 * Provides base config logic: load/save values from/to permanent storage.
 */
class CLOCK_COMMON_EXPORT ConfigBase : public SettingsStorageWrapper
{
  Q_OBJECT
public:
  /*!
   * Constructor
   * @param backend - settings storage backend
   * @param parent - parent object
   */
  explicit ConfigBase(SettingsStorage* backend, QObject* parent = nullptr);

  /*!
   * @brief Get option value.
   *
   * Tries to get value from backend, and if have no success returns option default value.
   * @param id - option id
   * @return option value
   * @see ConfigBase::GetDefaultValue(), SettingsStorage::GetValue()
   */
  QVariant GetValue(const int id) const;

public slots:
  /*!
   * Change option value on runtime.
   * @param id - option id
   * @param value - option value
   * @note permanent storage will not modified
   */
  void SetValue(const int id, const QVariant& value);

protected:
  /*!
   * Get string key for given option @a id
   * @note each key must be unique
   */
  virtual QString GetKey(const int id) const = 0;
  /*!
   * Get default value for given option @a id
   * @note every option must have valid default value
   */
  virtual QVariant GetDefaultValue(const int id) const = 0;
};

#endif // CONFIG_BASE_H
