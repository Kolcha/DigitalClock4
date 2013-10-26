#ifndef VECTOR_SKIN_H
#define VECTOR_SKIN_H

#include "base_skin.h"

/*!
 * @brief The VectorSkin class.
 *
 * VectorSkin class implements resize logic for vector images. Only SVG is supported.
 */
class VectorSkin : public BaseSkin {
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

#endif // VECTOR_SKIN_H
