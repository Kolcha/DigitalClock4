/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef PLUGIN_INFO_H
#define PLUGIN_INFO_H

#include <QMap>
#include <QString>
#include "plugin_base.h"

/*! plugin metadata fields */
enum FPluginInfo {
  PI_NAME,        /*!< plugin internal name */
  PI_VERSION,     /*!< plugin version */
  PI_AUTHOR,      /*!< plugin author */
  PI_EMAIL,       /*!< author's e-mail */
};

/*! plugin info type */
struct TPluginInfo {
  TPluginGUIInfo gui_info;              /*!< translatable plugin info */
  QMap<FPluginInfo, QString> metadata;  /*!< plugin metadata and other info */
};

#endif // PLUGIN_INFO_H
