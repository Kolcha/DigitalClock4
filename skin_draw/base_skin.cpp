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

#include "base_skin.h"

#include <QMap>

namespace skin_draw {

class CharImageCache : public IImageCache
{
public:
  QPixmap GetImage(const QString& str, int idx) override
  {
    auto iter = cache_.find(str[idx]);
    if (iter != cache_.end()) return iter.value();
    return QPixmap();
  }

  void AddImage(const QString& str, int idx, const QPixmap& image) override
  {
    if (!image) return;
    cache_[str[idx]] = image;
  }

  void Clear() override { cache_.clear(); }

private:
  QMap<QChar, QPixmap> cache_;
};


BaseSkin::BaseSkin() : cached_zoom_(1.0)
{
  img_cache_ = ImageCachePtr(new CharImageCache());
}

QPixmap BaseSkin::GetImage(const QString& str, int idx, Zoom zoom, bool cache)
{
  QPixmap result;
  if (idx >= str.length()) return result;

  if (str[idx] == '\n') return result;

  if (zoom == cached_zoom_) {
    result = img_cache_->GetImage(str, idx);
    if (!result) {
      result = ResizeImage(str, idx, zoom);
      if (!result.isNull() && cache) img_cache_->AddImage(str, idx, result);
    }
  } else {
    result = ResizeImage(str, idx, zoom);
    if (cache) {
      img_cache_->Clear();
      cached_zoom_ = zoom;
      img_cache_->AddImage(str, idx, result);
    }
  }

  return result;
}

} // namespace skin_draw
