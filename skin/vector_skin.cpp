#include <QSvgRenderer>
#include <QPainter>
#include "vector_skin.h"

VectorSkin::VectorSkin(const QString& skin_root)
  : BaseSkin(skin_root) {
}

VectorSkin::~VectorSkin() {
}

QPixmap* VectorSkin::ResizeImage(const QString& s, qreal zoom) {
  QSvgRenderer renderer(image_files_[s]);
  QPixmap* result = new QPixmap(renderer.defaultSize() * zoom);
  result->fill(Qt::black);
  QPainter painter(result);
  renderer.render(&painter);
  return result;
}
