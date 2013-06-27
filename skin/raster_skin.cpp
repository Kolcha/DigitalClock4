#include "raster_skin.h"

RasterSkin::RasterSkin(const QDir& skin_root)
  : BaseSkin(skin_root) {
  for (auto iter = image_files_.begin(); iter != image_files_.end(); ++iter) {
    orig_images_[iter.key()] = QPixmap(iter.value());
  }
}

QPixmapPtr RasterSkin::ResizeImage(const QString& s, qreal zoom) {
  QPixmap& original = orig_images_[s];
  QPixmapPtr result(new QPixmap());
  *result = original.scaled(original.size() * zoom,
                            Qt::KeepAspectRatio, Qt::SmoothTransformation);
  return result;
}
