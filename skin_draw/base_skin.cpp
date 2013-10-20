#include "base_skin.h"

BaseSkin::BaseSkin() : cached_zoom_(1.0) {
}

BaseSkin::~BaseSkin() {
}

void BaseSkin::GetInfo(TSkinInfo* info) {
  for (auto i = info_.begin(); i != info_.end(); ++i) {
    info->insert(i.key(), i.value());
  }
}

QPixmapPtr BaseSkin::GetImage(QChar ch, qreal zoom, bool cache) {
  QString s;
  CharToKey(ch, s);
  QPixmapPtr result;
  if (zoom == cached_zoom_) {
    result = image_cache_[s];
    if (!result) {
      result = ResizeImage(s, zoom);
      if (cache) image_cache_[s] = result;
    }
  } else {
    result = ResizeImage(s, zoom);
    if (cache) {
      image_cache_.clear();
      cached_zoom_ = zoom;
      image_cache_[s] = result;
    }
  }
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
    case 'a':
    case 'A':
      s = QString("am");
      break;
    case 'p':
    case 'P':
      s = QString("pm");
      break;
    default:
      s = QString(ch);
      break;
  }
}
