/*
    Digital Clock: skin draw engine
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

#ifndef SKIN_DRAW_RASTER_SKIN_H
#define SKIN_DRAW_RASTER_SKIN_H

#include "base_skin.h"

#include <QMap>

namespace skin_draw {

/*!
 * @brief Raster images support class.
 *
 * RasterSkin class implements resize logic for raster images. Many formats are supported,
 * see Qt documentation for details. PNG format is recommend.
 * @note There is no any load skin logic. You must inherit this class and implement your own
 * logic for skin resources loading. Load logic must fill RasterSkin::orig_images_ map.
 */
class SKIN_DRAWSHARED_EXPORT RasterSkin : public BaseSkin
{
protected:
  /*!
   * Resize image for given character.
   * @param str - text to render
   * @param idx - symbol index
   * @param zoom - requested zoom
   * @return pointer to resized image
   */
  QPixmap ResizeImage(const QString& str, int idx, Zoom zoom) override;
  /// map with original images
  QMap<QChar, QPixmap> orig_images_;
};

} // namespace skin_draw

#endif // SKIN_DRAW_RASTER_SKIN_H
