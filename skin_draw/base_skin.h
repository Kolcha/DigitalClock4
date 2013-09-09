#ifndef BASE_SKIN_H
#define BASE_SKIN_H

#include "iclock_skin.h"

class BaseSkin : public IClockSkin {
public:
  explicit BaseSkin();
  virtual ~BaseSkin();

  const TSkinInfo& GetInfo() const;
  QPixmapPtr GetImage(QChar ch, qreal zoom, bool cache);

protected:
  virtual QPixmapPtr ResizeImage(const QString& s, qreal zoom) = 0;
  TSkinInfo info_;

private:
  void CharToKey(QChar ch, QString& s);
  qreal cached_zoom_;
  QMap<QString, QPixmapPtr> image_cache_;
};

#endif // BASE_SKIN_H
