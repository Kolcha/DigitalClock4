#include "raster_skin.h"

RasterSkin::RasterSkin(const QDir& skin_root)
  : BaseSkin(skin_root) {
  for (auto iter = image_files_.begin(); iter != image_files_.end(); ++iter) {
    orig_images_[iter.key()] = new QImage(iter.value());
  }
}

RasterSkin::~RasterSkin() {
  for (auto iter = orig_images_.begin(); iter != orig_images_.end(); ++iter) {
    delete iter.value();
    iter.value() = 0;
  }
}

QImage* RasterSkin::ResizeImage(const QString& s, qreal zoom) {
  QImage* original = orig_images_[s];
  QImage* result = new QImage();
  *result = original->scaled(original->size() * zoom,
                             Qt::KeepAspectRatio, Qt::SmoothTransformation);
  return result;
}
