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

#ifndef SKIN_DRAW_BASE_SKIN_H
#define SKIN_DRAW_BASE_SKIN_H

#include "iskin.h"

#include "skin_draw_global.h"
#include "iimage_cache.h"

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
   * @param str - text to render
   * @param idx - symbol index
   * @param zoom - skin zoom
   * @param cache - is image will added to cache
   * @return pointer to result image
   */
  QPixmap GetImage(const QString& str, int idx, Zoom zoom, bool cache) override;

protected:
  /*!
   * Resize image for given character.
   * @param str - text to render
   * @param idx - symbol index
   * @param zoom - requested zoom
   * @return pointer to resized image
   * @note skin-specific method
   */
  virtual QPixmap ResizeImage(const QString& str, int idx, Zoom zoom) = 0;

  /// image chache object
  ImageCachePtr img_cache_;

private:
  Zoom cached_zoom_;
};

} // namespace skin_draw

#endif // SKIN_DRAW_BASE_SKIN_H
