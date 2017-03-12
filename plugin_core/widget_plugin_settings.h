/*
    Digital Clock: plugin core library
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

#ifndef PLUGIN_WIDGET_PLUGIN_SETTINGS_H
#define PLUGIN_WIDGET_PLUGIN_SETTINGS_H

#include <QString>
#include <QMap>
#include <QVariant>

#include "plugin_core_global.h"

namespace plugin {

/*!
 * Common widget plugin options.
 */
enum WidgetPluginOption {
  OPT_USE_CLOCK_FONT,         /*!< will plugin use same font as clock, bool, true/false */
  OPT_CUSTOM_FONT,            /*!< plugin font, if OPT_USE_CLOCK_FONT is not set, QFont */
  OPT_ZOOM_MODE,              /*!< controls widget zoom behaviour, one of ZoomMode values */
  OPT_SPACE_PERCENT,          /*!< percent of clock space to fill, int, [1-100] */
  OPT_WIDGET_LOCATION,        /*!< widget position, WidgetLocation, one of WidgetLocation values */
  OPT_ALIGNMENT,              /*!< plugin widgets alignment, Qt::Alignment, any allowed flags combination */
  OPT_USE_CUSTOM_COLOR,       /*!< will plugin use custom color or drawn as clock, bool, true/false */
  OPT_CUSTOM_COLOR            /*!< plugin color, if OPT_USE_CUSTOM_COLOR is set, QColor */
};

/*!
 * @brief Do WidgetPluginOption -> QString conversion.
 *
 * Generates unique string for each WidgetPluginOption value, which later will used as key
 * during setting save/load.
 * Generated string looks like filesystem path: "plugins/<plugin_name>/<option_name>"
 * @param opt - one of WidgetPluginOption
 * @param plg_name - plugin name, used to make keys unique for each plugin
 * @return option key (path)
 * @note All WidgetPluginOption values *must* be handled.
 * @see WidgetPluginBase::plg_name_
 */
QString PLUGIN_CORE_SHARED_EXPORT OptionKey(const WidgetPluginOption opt, const QString& plg_name);

/*!
 * Widget zoom modes.
 * @see WidgetPluginBase::avail_width_, WidgetPluginBase::CalculateZoom()
 */
enum ZoomMode {
  ZM_NOT_ZOOM,                /*!< do not zoom anything, draw as is */
  ZM_AUTOSIZE                 /*!< calculate suitable zoom to fill available space */
};

/*!
 * Available widget locations.
 */
enum WidgetLocation {
  WL_BOTTOM,                  /*!< widget will located under the clock */
  WL_RIGHT                    /*!< widget will located after the clock */
};

/*!
 * @brief Init common settings with default values.
 *
 * Fills given map with some default values for *each* element from WidgetPluginOption.
 * @param defaults - map to fill with values
 * @note This function is part of common stuff implementation, not for usage in plugins.
 * @see WidgetPluginOption, WidgetPluginBasePrivate
 */
void InitDefaults(QMap<WidgetPluginOption, QVariant>* defaults);

} // namespace plugin

#endif // PLUGIN_WIDGET_PLUGIN_SETTINGS_H
