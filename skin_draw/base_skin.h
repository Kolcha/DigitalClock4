#ifndef BASE_SKIN_H
#define BASE_SKIN_H

#include "iclock_skin.h"

/*!
 * @brief The BaseSkin class.
 *
 * This is a base class for skin classes. It implements get skin info and get image logic.
 * Get image logic:
 *  - try to get image from cache, if failed request for new image and add it to cache if needed
 *  - if try to request image with different zoom than cache has and try to add it to cache,
 * cache will be cleared and current skin zoom will be updated
 */
class BaseSkin : public IClockSkin {
public:
  /*! Constructor. */
  BaseSkin();
  /*! Destructor. */
  virtual ~BaseSkin();

  /*!
   * Get skin info.
   * @return skin info
   */
  const TSkinInfo& GetInfo() const;
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

  TSkinInfo info_;

private:
  qreal cached_zoom_;
  QMap<QChar, QPixmapPtr> image_cache_;
};

#endif // BASE_SKIN_H
