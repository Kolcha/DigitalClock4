#ifndef BASE_SKIN_H
#define BASE_SKIN_H

#include "iclock_skin.h"

class BaseSkin : public IClockSkin {
public:
  explicit BaseSkin(const QDir& skin_root);
  virtual ~BaseSkin();

  const TSkinInfo& GetInfo() const;
  const QImage& GetImage(QChar ch, qreal zoom, bool cache);

protected:
  virtual const QImage& ResizeImage(const QString& s, qreal zoom) = 0;
  QMap<QString, QString> image_files_;
  QImage result_;

private:
  void CharToKey(QChar ch, QString& s);
  TSkinInfo info_;
  qreal cached_zoom_;
  QMap<QString, QImage> image_cache_;
};

#endif // BASE_SKIN_H
