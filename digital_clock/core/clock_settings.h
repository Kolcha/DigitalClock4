/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2019  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef DIGITAL_CLOCK_CORE_CLOCK_SETTINGS_H
#define DIGITAL_CLOCK_CORE_CLOCK_SETTINGS_H

#include "config_base.h"

#include <QMap>

#include "settings_keys.h"

namespace digital_clock {
namespace core {

class ClockSettings : public ConfigBase
{
public:
  explicit ClockSettings(SettingsStorage* backend, QObject* parent = nullptr);

  // temporary added only for compatibility
  // TODO: remove
  QMap<Option, QVariant> GetSettings() const;

protected:
  QString GetKey(const int id) const override;
  QVariant GetDefaultValue(const int id) const override;
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_CLOCK_SETTINGS_H
