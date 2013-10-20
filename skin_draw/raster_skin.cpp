#include <QSettings>
#include "raster_skin.h"

RasterSkin::RasterSkin(const QDir& skin_root) {
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
    orig_images_[key] = QPixmap(skin_root.filePath(config.value("files/" + key).toString()));
  }
  orig_images_[QString("s1")] = QPixmap(skin_root.filePath(config.value("files/s1").toString()));
  orig_images_[QString("s2")] = QPixmap(skin_root.filePath(config.value("files/s2").toString()));
  orig_images_[QString("am")] = QPixmap(skin_root.filePath(config.value("files/am").toString()));
  orig_images_[QString("pm")] = QPixmap(skin_root.filePath(config.value("files/pm").toString()));
}

QPixmapPtr RasterSkin::ResizeImage(const QString& s, qreal zoom) {
  QPixmap& original = orig_images_[s];
  if (original.isNull()) return QPixmapPtr();

  QPixmapPtr result(new QPixmap());
  *result = original.scaled(original.size() * zoom, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  return result;
}
