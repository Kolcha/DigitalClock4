/*
    Digital Clock: plugin core library
    Copyright (C) 2017-2019  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef ISKIN_USER_PLUGIN_H
#define ISKIN_USER_PLUGIN_H

#include "iclock_plugin.h"

#include "iskin.h"
#include "plugin_core_global.h"

/*!
 * @brief Skin user plugin interface.
 *
 * This interface just adjust basic clock plugin interface and adds ability
 * to track clock skin changes and use current skin for drawing in plugins.
 * @see IClockPlugin
 */
class PLUGIN_CORE_SHARED_EXPORT ISkinUserPlugin : public IClockPlugin
{
  Q_OBJECT

public slots:
  /*!
   * @brief Skin changes listener.
   *
   * This slot is called in 2 cases:
   * @li exact one time on plugin initoalization, to pass current clock @a skin
   * @li on any clock skin change, new @a skin will be passed as argument
   */
  virtual void SetSkin(skin_draw::ISkin::SkinPtr skin) = 0;
};

/*! Skin user plugin interface IID */
#define SKIN_USER_PLUGIN_INTERFACE_IID   "nick-korotysh.digital-clock.skin-user-plugin"
Q_DECLARE_INTERFACE(ISkinUserPlugin, SKIN_USER_PLUGIN_INTERFACE_IID)

#endif // ISKIN_USER_PLUGIN_H
