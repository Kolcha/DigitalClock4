/*
    Digital Clock: variable translucency plugin
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

#ifndef VAR_TRANSLUCENCY_VAR_TRANSLUCENCY_H
#define VAR_TRANSLUCENCY_VAR_TRANSLUCENCY_H

#include "iclock_plugin.h"
#include "iplugin_init.h"

namespace var_translucency {

class IOpacityChanger
{
public:
  virtual ~IOpacityChanger() = default;

  virtual void setOpacity(const qreal opacity) = 0;
  virtual qreal opacity() const = 0;

  virtual IOpacityChanger& operator ++() = 0;
};

class VarTranslucency : public ISettingsPlugin, public ISettingsPluginInit
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "var_translucency.json")
  Q_INTERFACES(IClockPlugin ISettingsPlugin ISettingsPluginInit)

public:
  VarTranslucency();
  void Init(const QMap<Option, QVariant>& current_settings) override;

public slots:
  void Start() override;
  void Stop() override;
  void TimeUpdateListener() override;

private:
  qreal old_opacity_;
  IOpacityChanger* changer_;
};

} // namespace var_translucency

#endif // VAR_TRANSLUCENCY_VAR_TRANSLUCENCY_H
