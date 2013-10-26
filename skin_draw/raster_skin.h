#ifndef RASTER_SKIN_H
#define RASTER_SKIN_H

#include "base_skin.h"

/*!
 * @brief The RasterSkin class.
 *
 * RasterSkin class implements resize logic for raster images.
 */
class SKIN_DRAWSHARED_EXPORT RasterSkin : public BaseSkin {
public:
  /*! Virtual destructor. */
  virtual ~RasterSkin();

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

#endif // RASTER_SKIN_H
