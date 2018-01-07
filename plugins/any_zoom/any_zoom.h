/*
    Digital Clock: any zoom plugin
    Copyright (C) 2013-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef ANY_ZOOM_ANY_ZOOM_H
#define ANY_ZOOM_ANY_ZOOM_H

#include "iclock_plugin.h"
#include "iplugin_init.h"

#include "zoom_type.hpp"

namespace any_zoom {

class AnyZoom : public ISettingsPlugin, ISettingsPluginInit
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "any_zoom.json")
  Q_INTERFACES(IClockPlugin ISettingsPlugin ISettingsPluginInit)

public:
  AnyZoom();
  void Init(const QMap<Option, QVariant>& current_settings) override;

public slots:
  void Start() override;
  void Stop() override;
  void Configure() override;
  void SettingsListener(Option option, const QVariant& value) override;

private slots:
  void TrackChange(const ::skin_draw::Zoom& zoom);
  void RevertSettings();

private:
  ::skin_draw::Zoom LoadZoom() const;

private:
  bool is_enabled_;
  qreal last_zoom_;
};

} // namespace any_zoom

#endif // ANY_ZOOM_ANY_ZOOM_H
