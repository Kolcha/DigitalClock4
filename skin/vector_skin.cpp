#include <QSvgRenderer>
#include <QPainter>
#include "vector_skin.h"

VectorSkin::VectorSkin(const QDir& skin_root)
  : BaseSkin(skin_root) {
}

QImage* VectorSkin::ResizeImage(const QString& s, qreal zoom) {
  QSvgRenderer renderer(image_files_[s]);
  QImage* result = new QImage(renderer.defaultSize() * zoom, QImage::Format_ARGB32_Premultiplied);
  QPainter painter(result);
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(result->rect(), Qt::transparent);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  renderer.render(&painter);
  return result;
}
