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

#include "base_skin.h"

namespace skin_draw {

BaseSkin::BaseSkin() : cached_zoom_(1.0), device_pixel_ratio_(1.0)
{
}

ISkin::QPixmapPtr BaseSkin::GetImage(QChar ch, qreal zoom, bool cache)
{
  QPixmapPtr result;
  if (ch == '\n') {
    return QPixmapPtr(new QPixmap());
  }
  if (zoom == cached_zoom_) {
    result = image_cache_[ch];
    if (!result) {
      result = ResizeImage(ch, zoom);
      if (cache) image_cache_[ch] = result;
    }
  } else {
    result = ResizeImage(ch, zoom);
    if (cache) {
      image_cache_.clear();
      cached_zoom_ = zoom;
      image_cache_[ch] = result;
    }
  }
  return result;
}

qreal BaseSkin::GetDevicePixelRatio() const
{
  return device_pixel_ratio_;
}

void BaseSkin::SetDevicePixelRatio(qreal ratio)
{
  device_pixel_ratio_ = ratio;
}

} // namespace skin_draw
