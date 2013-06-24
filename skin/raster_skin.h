#ifndef RASTER_SKIN_H
#define RASTER_SKIN_H

#include "base_skin.h"

class RasterSkin : public BaseSkin {
public:
  RasterSkin(const QString& skin_root);
  ~RasterSkin();

protected:
  QPixmap* ResizeImage(const QString& s, qreal zoom);

private:
  QMap<QString, QPixmap*> orig_images_;
};

#endif // RASTER_SKIN_H
