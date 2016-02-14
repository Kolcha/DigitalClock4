#ifndef BASE_SKIN_H
#define BASE_SKIN_H

#include "iskin.h"
#include <QMap>

namespace skin_draw {

/*!
 * @brief Base class for all skin classes.
 *
 * It implements get image logic and images cache.
 * Get image logic:
 * @li try to get image from cache, if failed request for new image and add it to cache if needed
 * @li if try to request image with different zoom than cache has and try to add it to cache,
 * cache will be cleared and current skin zoom will be updated
 *
 * @see BaseSkin::ResizeImage()
 */
class SKIN_DRAWSHARED_EXPORT BaseSkin : public ISkin {
public:
  /*! Constructor. */
  BaseSkin();

  /*!
   * Get image for specified symbol.
   * @param ch - symbol for which need to get image
   * @param zoom - skin zoom
   * @param cache - is image add to cache
   * @return pointer to result image
   */
  QPixmapPtr GetImage(QChar ch, qreal zoom, bool cache);

protected:
  /*!
   * Resize image for given character.
   * @param ch - requested charater
   * @param zoom - requested zoom
   * @return pointer to resized image
   * @note skin-specific method
   */
  virtual QPixmapPtr ResizeImage(QChar ch, qreal zoom) = 0;
  /// device pixel ratio, see Qt documentation for details
  qreal device_pixel_ratio_;

private:
  qreal cached_zoom_;
  QMap<QChar, QPixmapPtr> image_cache_;
};

} // namespace skin_draw

#endif // BASE_SKIN_H
