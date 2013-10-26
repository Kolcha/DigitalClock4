#include <QSettings>
#include "clock_vector_skin.h"

ClockVectorSkin::ClockVectorSkin(const QDir& skin_root) {
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
    image_files_[key[0]] = skin_root.filePath(config.value("files/" + key, "invalid").toString());
  }
  image_files_[':'] = skin_root.filePath(config.value("files/s1", "invalid").toString());
  image_files_[' '] = skin_root.filePath(config.value("files/s2", "invalid").toString());
  image_files_['a'] = skin_root.filePath(config.value("files/am", "invalid").toString());
  image_files_['p'] = skin_root.filePath(config.value("files/pm", "invalid").toString());
  image_files_['A'] = image_files_['a'];
  image_files_['P'] = image_files_['p'];
}
