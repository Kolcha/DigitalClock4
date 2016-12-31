/*
    Digital Clock: skin draw engine
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

#ifndef SKIN_DRAW_IIMAGE_CACHE_H
#define SKIN_DRAW_IIMAGE_CACHE_H

#include "iskin.h"

#include <QSharedPointer>

namespace skin_draw {

/*!
 * @brief interface for all image cache classes.
 *
 * Each skin class *must* have resize/rendered images cache.
 * This class provides common interface for all these caches.
 */
class IImageCache
{
public:
  /*! Virtual destructor. */
  virtual ~IImageCache() {}

  /*!
   * Get image from cache.
   * @param str - text to render
   * @param idx - symbol index
   * @return cached image, returns invalid pointer if image was not found
   */
  virtual ISkin::QPixmapPtr GetImage(const QString& str, int idx) = 0;
  /*!
   * Add @a image to cache.
   */
  virtual void AddImage(const QString& str, int idx, const ISkin::QPixmapPtr& image) = 0;

  /*!
   * Drop all cached images.
   */
  virtual void Clear() = 0;
};

/// image cache pointer type
typedef QSharedPointer<IImageCache> ImageCachePtr;

} // namespace skin_draw

#endif // SKIN_DRAW_IIMAGE_CACHE_H
