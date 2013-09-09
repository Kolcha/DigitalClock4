#ifndef ICLOCK_SKIN_H
#define ICLOCK_SKIN_H

#include "skin_draw_global.h"

#include <QDir>
#include <QMap>
#include <QString>
#include <QPixmap>
#include <QSharedPointer>

// skin info fields
enum FSkinInfo {
  SI_NAME,        // skin name
  SI_VERSION,     // skin version
  SI_AUTHOR,      // skin author
  SI_EMAIL,       // author's e-mail
  SI_COMMENT      // skin description
};
// skin info type
typedef QMap<FSkinInfo, QString> TSkinInfo;

typedef QSharedPointer<QPixmap> QPixmapPtr;

class IClockSkin {
public:
  virtual ~IClockSkin() {}

  virtual const TSkinInfo& GetInfo() const = 0;
  virtual QPixmapPtr GetImage(QChar ch, qreal zoom, bool cache) = 0;
};

SKIN_DRAWSHARED_EXPORT IClockSkin* CreateSkin(const QDir& skin_root);

#endif // ICLOCK_SKIN_H
