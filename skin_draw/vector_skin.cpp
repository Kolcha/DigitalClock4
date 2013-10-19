#include <QSettings>
#include <QSvgRenderer>
#include <QPainter>
#include "vector_skin.h"

VectorSkin::VectorSkin(const QDir& skin_root) {
  QSettings config(skin_root.filePath("skin.ini"), QSettings::IniFormat);
  // load info
  info_[SI_NAME] = config.value("info/name", "unknown").toString();
  info_[SI_VERSION] = config.value("info/version", "unknown").toString();
  info_[SI_AUTHOR] = config.value("info/author", "unknown").toString();
  info_[SI_EMAIL] = config.value("info/e-mail", "unknown").toString();
  info_[SI_COMMENT] = config.value("info/comment", "unknown").toString();
  // load image files
  for (int i = 0; i < 10; ++i) {
    QString key = QString::number(i);
    image_files_[key] = skin_root.filePath(config.value("files/" + key).toString());
  }
  image_files_[QString("s1")] = skin_root.filePath(config.value("files/s1").toString());
  image_files_[QString("s2")] = skin_root.filePath(config.value("files/s2").toString());
}

QPixmapPtr VectorSkin::ResizeImage(const QString& s, qreal zoom) {
  QString& img_file = image_files_[s];
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
