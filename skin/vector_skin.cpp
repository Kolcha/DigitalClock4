#include <QSvgRenderer>
#include <QPainter>
#include "vector_skin.h"

VectorSkin::VectorSkin(const QString& skin_root)
  : BaseSkin(skin_root) {
}

VectorSkin::~VectorSkin() {
}

const QImage& VectorSkin::ResizeImage(const QString& s, qreal zoom) {
  QSvgRenderer renderer(image_files_[s]);
  result_ = QImage(renderer.defaultSize() * zoom,
                   QImage::Format_ARGB32_Premultiplied);
  QPainter painter(&result_);
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(result_.rect(), Qt::transparent);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  renderer.render(&painter);
  return result_;
}
