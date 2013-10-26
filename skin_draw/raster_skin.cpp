#include "raster_skin.h"

RasterSkin::~RasterSkin() {
}

ISkin::QPixmapPtr RasterSkin::ResizeImage(QChar ch, qreal zoom) {
  QPixmap& original = orig_images_[ch];
  if (original.isNull()) return QPixmapPtr();

  QPixmapPtr result(new QPixmap());
  *result = original.scaled(original.size() * zoom, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  return result;
}
