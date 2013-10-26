#ifndef ICLOCK_SKIN_H
#define ICLOCK_SKIN_H

#include <QMap>
#include <QString>
#include <QPixmap>
#include <QSharedPointer>
#include "skin_draw_global.h"

/*!
 * @brief The IClockSkin class.
 *
 * Clock skin interface.
 */
class SKIN_DRAWSHARED_EXPORT IClockSkin {
public:
  /*! Virtual destructor. */
  virtual ~IClockSkin() {}

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
  /// clock skin pointer type
  typedef QSharedPointer<IClockSkin> ClockSkinPtr;
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

#endif // ICLOCK_SKIN_H
