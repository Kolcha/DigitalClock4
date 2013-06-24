#ifndef RASTER_SKIN_H
#define RASTER_SKIN_H

#include "base_skin.h"

class RasterSkin : public BaseSkin {
public:
  RasterSkin(const QString& skin_root);
  ~RasterSkin();

  QPixmap* GetImage(const QString& s, qreal zoom, bool cache);

private:
  QPixmap* ResizeImage(const QString& s, qreal zoom);
  QMap<QString, QPixmap*> orig_images_;
};

#endif // RASTER_SKIN_H
