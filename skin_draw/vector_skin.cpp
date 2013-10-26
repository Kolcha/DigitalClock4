#include <QFile>
#include <QSvgRenderer>
#include <QPainter>
#include "vector_skin.h"

VectorSkin::~VectorSkin() {
}

IClockSkin::QPixmapPtr VectorSkin::ResizeImage(QChar ch, qreal zoom) {
  QString& img_file = image_files_[ch];
  if (!QFile::exists(img_file)) return QPixmapPtr();

  QSvgRenderer renderer(img_file);
  QPixmapPtr result(new QPixmap(renderer.defaultSize() * zoom));
  QPainter painter(result.data());
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(result->rect(), Qt::transparent);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  renderer.render(&painter);
  return result;
}
