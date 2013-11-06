#ifndef ISKIN_H
#define ISKIN_H

#include <QPixmap>
#include <QSharedPointer>
#include "skin_draw_global.h"

/*!
 * @brief The ISkin class.
 *
 * Skin interface.
 */
class SKIN_DRAWSHARED_EXPORT ISkin {
public:
  /*! Virtual destructor. */
  virtual ~ISkin() {}

  /// skin pointer type
  typedef QSharedPointer<ISkin> SkinPtr;
  /// QPixmap shared pointer type
  typedef QSharedPointer<QPixmap> QPixmapPtr;

  /*!
   * Get image for specified symbol.
   * @param ch - symbol for which need to get image
   * @param zoom - skin zoom
   * @param cache - is image add to cache
   * @return pointer to result image
   */
  virtual QPixmapPtr GetImage(QChar ch, qreal zoom, bool cache) = 0;
};

#endif // ISKIN_H
