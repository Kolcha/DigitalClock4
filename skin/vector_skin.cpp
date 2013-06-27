#include <QSvgRenderer>
#include <QPainter>
#include "vector_skin.h"

VectorSkin::VectorSkin(const QDir& skin_root)
  : BaseSkin(skin_root) {
}

QPixmapPtr VectorSkin::ResizeImage(const QString& s, qreal zoom) {
  QSvgRenderer renderer(image_files_[s]);
  QPixmapPtr result(new QPixmap(renderer.defaultSize() * zoom));
  QPainter painter(result.data());
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(result->rect(), Qt::transparent);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  renderer.render(&painter);
  return result;
}
