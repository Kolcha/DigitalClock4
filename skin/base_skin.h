#ifndef BASE_SKIN_H
#define BASE_SKIN_H

#include <QStringList>
#include "iclock_skin.h"

class BaseSkin : public IClockSkin {
public:
  BaseSkin(const QString& skin_root);
  virtual ~BaseSkin();

  const TSkinInfo& GetInfo() const;
  QPixmap* GetImage(const QString& s, qreal zoom, bool cache);

protected:
  virtual QPixmap* ResizeImage(const QString& s, qreal zoom) = 0;
  QMap<QString, QString> image_files_;

private:
  void ClearCache();
  void GenImgKeys();
  void LoadConfig();
  QString skin_root_;
  TSkinInfo info_;
  QStringList image_keys_;
  qreal cached_zoom_;
  QMap<QString, QPixmap*> image_cache_;
};

#endif // BASE_SKIN_H
