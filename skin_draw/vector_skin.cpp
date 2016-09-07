#include "vector_skin.h"

#include <QFile>
#include <QSvgRenderer>
#include <QPainter>

namespace skin_draw {

ISkin::QPixmapPtr VectorSkin::ResizeImage(QChar ch, qreal zoom)
{
  QString& img_file = image_files_[ch];
  if (!QFile::exists(img_file)) return QPixmapPtr();

  QSvgRenderer renderer(img_file);
  QPixmapPtr result(new QPixmap(renderer.defaultSize() * zoom * device_pixel_ratio_));
  QPainter painter(result.data());
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(result->rect(), Qt::transparent);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  renderer.render(&painter);
  painter.end();
  result->setDevicePixelRatio(device_pixel_ratio_);
  return result;
}

} // namespace skin_draw
