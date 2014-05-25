#include "base_skin.h"

namespace skin_draw {

BaseSkin::BaseSkin() : ratio_(1.0), cached_zoom_(1.0) {
}

ISkin::QPixmapPtr BaseSkin::GetImage(QChar ch, qreal zoom, bool cache) {
  QPixmapPtr result;
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

void BaseSkin::SetDevicePixelRatio(qreal new_ratio) {
  ratio_ = new_ratio;
}

} // namespace skin_draw
