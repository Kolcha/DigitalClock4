#include <QDir>
#include <QSettings>
#include "base_skin.h"

BaseSkin::BaseSkin(const QString &skin_root)
  : skin_root_(skin_root), cached_zoom_(1.0) {
  GenImgKeys();
  for (auto& key : image_keys_) {
    image_cache_[key] = 0;
  }
  LoadConfig();
}

BaseSkin::~BaseSkin() {
  ClearCache();
}

const TSkinInfo& BaseSkin::GetInfo() const {
  return info_;
}

QPixmap* BaseSkin::GetImage(QChar ch, qreal zoom, bool cache) {
  QString s;
  CharToKey(ch, s);
  QPixmap* result = 0;
  if (zoom == cached_zoom_) {
    result = image_cache_[s];
    if (!result) result = ResizeImage(s, zoom);
  } else {
    result = ResizeImage(s, zoom);
    if (cache) {
      ClearCache();
      cached_zoom_ = zoom;
    }
  }
  if (cache) image_cache_[s] = result;
  return result;
}

void BaseSkin::ClearCache() {
  for (auto& key : image_keys_) {
    delete image_cache_[key];
    image_cache_[key] = 0;
  }
}

void BaseSkin::GenImgKeys() {
  for (qint8 i = 1; i < 10; ++i) {
    image_keys_.append(QString::number(i));
  }
  image_keys_.push_back(QString("s1"));
  image_keys_.push_back(QString("s2"));
}

void BaseSkin::LoadConfig() {
  QSettings config(QDir(skin_root_).filePath("skin.ini"), QSettings::IniFormat);
  // load info
  info_[SI_NAME] = config.value("info/name", "unknown").toString();
  info_[SI_VERSION] = config.value("info/version", "unknown").toString();
  info_[SI_AUTHOR] = config.value("info/author", "unknown").toString();
  info_[SI_EMAIL] = config.value("info/e-mail", "unknown").toString();
  info_[SI_COMMENT] = config.value("info/comment", "unknown").toString();
  info_[SI_TYPE] = config.value("info/type", "unknown").toString();
  // load image files
  for (auto& key : image_keys_) {
    image_files_[key] = config.value("files/" + key, "").toString();
  }
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
