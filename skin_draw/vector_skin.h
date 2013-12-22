#ifndef VECTOR_SKIN_H
#define VECTOR_SKIN_H

#include "base_skin.h"

namespace SKIN_DRAW_NAMESPACE {

/*!
 * @brief Vector images support class.
 *
 * VectorSkin class implements resize logic for vector images. Only SVG is supported.
 * @note There is no any load skin logic. You must inherit this class and implement your own
 * logic for skin resources loading. Load logic must fill VectorSkin::image_files_ map.
 */
class SKIN_DRAWSHARED_EXPORT VectorSkin : public BaseSkin {
public:
  /*! Virtual destructor. */
  virtual ~VectorSkin();

protected:
  /*!
   * Resize image for given character.
   * @param ch - requested charater
   * @param zoom - requested zoom
   * @return pointer to resized image
   */
  QPixmapPtr ResizeImage(QChar ch, qreal zoom);
  /// map with paths to original images
  QMap<QChar, QString> image_files_;
};

} // SKIN_DRAW_NAMESPACE

#endif // VECTOR_SKIN_H
