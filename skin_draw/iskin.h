#ifndef ISKIN_H
#define ISKIN_H

#include <QMap>
#include <QString>
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

  /*! skin info fields */
  enum FSkinInfo {
    SI_NAME,        /*!< skin name */
    SI_VERSION,     /*!< skin version */
    SI_AUTHOR,      /*!< skin author */
    SI_EMAIL,       /*!< author's e-mail */
    SI_COMMENT      /*!< skin description */
  };
  /// skin info type
  typedef QMap<FSkinInfo, QString> TSkinInfo;
  /// skin pointer type
  typedef QSharedPointer<ISkin> SkinPtr;
  /// QPixmap shared pointer type
  typedef QSharedPointer<QPixmap> QPixmapPtr;

  /*!
   * Get skin info.
   * @return skin info
   */
  virtual const TSkinInfo& GetInfo() const = 0;
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
