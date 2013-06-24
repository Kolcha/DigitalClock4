#include "raster_skin.h"

RasterSkin::RasterSkin(const QString& skin_root)
  : BaseSkin(skin_root) {
  for (auto iter = image_files_.begin(); iter != image_files_.end(); ++iter) {
    orig_images_[iter.key()] = new QPixmap(iter.value());
  }
}

RasterSkin::~RasterSkin() {
  for (auto iter = orig_images_.begin(); iter != orig_images_.end(); ++iter) {
    delete iter.value();
  }
}

QPixmap* RasterSkin::GetImage(const QString& s, qreal zoom, bool cache) {
  QPixmap* result = 0;
  if (zoom == cached_zoom_) {
    result = image_cache_[s];
    if (!result) result = ResizeImage(s, zoom);
  } else {
    result = ResizeImage(s, zoom);
    if (cache) {
      ClearCache();
      cached_zoom_ = zoom;
    }
  }
  if (cache) image_cache_[s] = result;
  return result;
}

QPixmap* RasterSkin::ResizeImage(const QString& s, qreal zoom) {
  QPixmap* result = new QPixmap();
  QPixmap* original = orig_images_[s];
  *result = original->scaled(original->size() * zoom,
                             Qt::KeepAspectRatio, Qt::SmoothTransformation);
  return result;
}
