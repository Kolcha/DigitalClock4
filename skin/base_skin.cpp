#include <QSettings>
#include "base_skin.h"

BaseSkin::BaseSkin(const QDir& skin_root)
  : cached_zoom_(1.0) {
  QSettings config(skin_root.filePath("skin.ini"), QSettings::IniFormat);
  // load info
  info_[SI_NAME] = config.value("info/name", "unknown").toString();
  info_[SI_VERSION] = config.value("info/version", "unknown").toString();
  info_[SI_AUTHOR] = config.value("info/author", "unknown").toString();
  info_[SI_EMAIL] = config.value("info/e-mail", "unknown").toString();
  info_[SI_COMMENT] = config.value("info/comment", "unknown").toString();
  info_[SI_TYPE] = config.value("info/type", "unknown").toString();
  // load image files
  for (int i = 0; i < 10; ++i) {
    QString key = QString::number(i);
    image_files_[key] = skin_root.filePath(config.value("files/" + key).toString());
  }
  image_files_[QString("s1")] = skin_root.filePath(config.value("files/s1").toString());
  image_files_[QString("s2")] = skin_root.filePath(config.value("files/s2").toString());
}

BaseSkin::~BaseSkin() {
}

const TSkinInfo& BaseSkin::GetInfo() const {
  return info_;
}

const QImage& BaseSkin::GetImage(QChar ch, qreal zoom, bool cache) {
  QString s;
  CharToKey(ch, s);
  QImage& result = QImage();
  if (zoom == cached_zoom_) {
    result = image_cache_[s];
    if (result.isNull()) result = ResizeImage(s, zoom);
  } else {
    result = ResizeImage(s, zoom);
    if (cache) {
      image_cache_.clear();
      cached_zoom_ = zoom;
    }
  }
  if (cache) image_cache_[s] = result;
  return result;
}

void BaseSkin::CharToKey(QChar ch, QString& s) {
  switch (ch.toLatin1()) {
    case ':':
      s = QString("s1");
      break;
    case ' ':
      s = QString("s2");
      break;
    default:
      s = QString(ch);
      break;
  }
}
