#include "base_skin.h"

#include <QGuiApplication>
#include <QScreen>

namespace skin_draw {

BaseSkin::BaseSkin() : cached_zoom_(1.0)
{
  device_pixel_ratio_ = QGuiApplication::primaryScreen()->devicePixelRatio();
}

ISkin::QPixmapPtr BaseSkin::GetImage(QChar ch, qreal zoom, bool cache)
{
  QPixmapPtr result;
  if (ch == '\n') {
    result = QPixmapPtr(new QPixmap());
    result->setDevicePixelRatio(device_pixel_ratio_);
    return result;
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

} // namespace skin_draw
