#include <QDir>
#include <QSettings>
#include "base_skin.h"

BaseSkin::BaseSkin(const QString &skin_root)
  : cached_zoom_(1.0), skin_root_(skin_root) {
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
