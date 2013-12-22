#include "base_skin.h"

namespace SKIN_DRAW_NAMESPACE {

BaseSkin::BaseSkin() : cached_zoom_(1.0) {
}

BaseSkin::~BaseSkin() {
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

} // SKIN_DRAW_NAMESPACE
