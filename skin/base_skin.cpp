#include <QSettings>
#include "base_skin.h"

BaseSkin::BaseSkin(const QString &skin_root)
  : skin_root_(skin_root) {   // TODO: check last separator
  GenImgKeys();
  for (auto& key : image_keys_) {
    image_cache_[key] = 0;
  }
  LoadConfig();
}

BaseSkin::~BaseSkin() {
  for (auto& key : image_keys_) {
    delete image_cache_[key];
    image_cache_[key] = 0;
  }
}

const TSkinInfo& BaseSkin::GetInfo() const {
  return info_;
}

void BaseSkin::GenImgKeys() {
  for (qint8 i = 1; i < 10; ++i) {
    image_keys_.append(QString::number(i));
  }
  image_keys_.push_back(QString("s1"));
  image_keys_.push_back(QString("s2"));
}

void BaseSkin::LoadConfig() {
  QSettings config(skin_root_ + "skin.ini", QSettings::IniFormat);
  // load info
  info_[SI_NAME] = config.value("info/name", "unknown").toString();
  info_[SI_VERSION] = config.value("info/version", "unknown").toString();
  info_[SI_AUTHOR] = config.value("info/author", "unknown").toString();
  info_[SI_EMAIL] = config.value("info/e-mail", "unknown").toString();
  info_[SI_COMMENT] = config.value("info/comment", "unknown").toString();
  info_[SI_TYPE] = config.value("info/type", "unknown").toString();
  // load image files
  for (auto& key : image_keys_) {
    image_files_.append(config.value("files/" + key, "").toString());
  }
}
