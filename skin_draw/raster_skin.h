#ifndef RASTER_SKIN_H
#define RASTER_SKIN_H

#include "base_skin.h"

namespace skin_draw {

/*!
 * @brief Raster images support class.
 *
 * RasterSkin class implements resize logic for raster images. Many formats are supported,
 * see Qt documentation for details. PNG format is recommend.
 * @note There is no any load skin logic. You must inherit this class and implement your own
 * logic for skin resources loading. Load logic must fill RasterSkin::orig_images_ map.
 */
class SKIN_DRAWSHARED_EXPORT RasterSkin : public BaseSkin {
protected:
  /*!
   * Resize image for given character.
   * @param ch - requested charater
   * @param zoom - requested zoom
   * @return pointer to resized image
   */
  QPixmapPtr ResizeImage(QChar ch, qreal zoom);
  /// map with original images
  QMap<QChar, QPixmap> orig_images_;
};

} // namespace skin_draw

#endif // RASTER_SKIN_H
