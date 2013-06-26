#include "raster_skin.h"

RasterSkin::RasterSkin(const QString& skin_root)
  : BaseSkin(skin_root) {
  for (auto iter = image_files_.begin(); iter != image_files_.end(); ++iter) {
    orig_images_[iter.key()] = QImage(iter.value());
  }
}

const QImage& RasterSkin::ResizeImage(const QString& s, qreal zoom) {
  const QImage& original = orig_images_[s];
  result_ = original.scaled(original.size() * zoom,
                            Qt::KeepAspectRatio, Qt::SmoothTransformation);
  return result_;
}
