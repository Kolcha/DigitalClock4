#ifndef RASTER_SKIN_H
#define RASTER_SKIN_H

#include "base_skin.h"

/*!
 * @brief The RasterSkin class.
 *
 * RasterSkin class implements resize logic for raster images.
 */
class RasterSkin : public BaseSkin {
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
  /// Map with original images. This must be filled in LoadData() method.
  QMap<QChar, QPixmap> orig_images_;
};

#endif // RASTER_SKIN_H
