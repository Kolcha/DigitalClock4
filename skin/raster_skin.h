#ifndef RASTER_SKIN_H
#define RASTER_SKIN_H

#include "base_skin.h"

class RasterSkin : public BaseSkin {
public:
  explicit RasterSkin(const QDir& skin_root);

protected:
  const QImage& ResizeImage(const QString& s, qreal zoom);

private:
  QMap<QString, QImage> orig_images_;
};

#endif // RASTER_SKIN_H
