/*
    Digital Clock: skin draw engine
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

#ifndef SKIN_DRAW_ISKIN_H
#define SKIN_DRAW_ISKIN_H

#include <QPixmap>
#include <QSharedPointer>

#include "zoom_type.hpp"

namespace skin_draw {

/*!
 * @brief Skin interface.
 *
 * This class provides interface for all supported skin types:
 *   - raster image skin
 *   - vector image skin
 *   - text skin
 *
 * This interface is used by skin draw engine.
 * @see RasterSkin, VectorSkin, TextSkin, SkinDrawer
 */
class ISkin
{
public:
  /*! Virtual destructor. */
  virtual ~ISkin() = default;

  /// skin pointer type
  typedef QSharedPointer<ISkin> SkinPtr;

  /*!
   * Get image for specified symbol.
   * @param str - text to render
   * @param idx - symbol index
   * @param zoom - skin zoom
   * @param cache - is image add to cache
   * @return pointer to result image
   */
  virtual QPixmap GetImage(const QString& str, int idx, Zoom zoom, bool cache) = 0;

  /*!
   * Get device pixel ratio for current skin, see Qt documentation for details.
   * @return device pixel ratio for skin as a floating point number.
   */
  virtual qreal GetDevicePixelRatio() const = 0;
  /*!
   * @brief Explicitly set device pixel ratio for skin.
   *
   * There is no any "autodetect" for current device pixel ratio, it must be set explicitly
   * before any usage of created skin.
   * @param ratio - device pixel ratio to set
   * @note default device pixel ratio value is 1.0
   */
  virtual void SetDevicePixelRatio(qreal ratio) = 0;
};

} // namespace skin_draw

#endif // SKIN_DRAW_ISKIN_H
