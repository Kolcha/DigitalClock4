/*
    Digital Clock: skin draw engine
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

#ifndef BASE_SKIN_H
#define BASE_SKIN_H

#include "iskin.h"
#include <QMap>
#include "skin_draw_global.h"

namespace skin_draw {

/*!
 * @brief Base class for all skin classes.
 *
 * It implements get image logic and images cache.
 * Get image logic:
 *   - try to get image from cache, if failed request for new image and add it to cache if needed
 *   - if try to request image with different zoom than cache has and try to add it to cache,
 * cache will be cleared and current skin zoom will be updated
 *
 * @see BaseSkin::ResizeImage()
 */
class SKIN_DRAWSHARED_EXPORT BaseSkin : public ISkin
{
public:
  /*! Constructor. */
  BaseSkin();

  /*!
   * Get image for specified symbol.
   * @param ch - symbol for which need to get image
   * @param zoom - skin zoom
   * @param cache - is image add to cache
   * @return pointer to result image
   */
  QPixmapPtr GetImage(QChar ch, qreal zoom, bool cache);

protected:
  /*!
   * Resize image for given character.
   * @param ch - requested charater
   * @param zoom - requested zoom
   * @return pointer to resized image
   * @note skin-specific method
   */
  virtual QPixmapPtr ResizeImage(QChar ch, qreal zoom) = 0;
  /// device pixel ratio, see Qt documentation for details
  qreal device_pixel_ratio_;

private:
  qreal cached_zoom_;
  QMap<QChar, QPixmapPtr> image_cache_;
};

} // namespace skin_draw

#endif // BASE_SKIN_H
