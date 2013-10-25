#ifndef ICLOCK_SKIN_H
#define ICLOCK_SKIN_H

#include "skin_draw_global.h"

#include <QDir>
#include <QMap>
#include <QFont>
#include <QString>
#include <QPixmap>
#include <QSharedPointer>

// TODO: add comments
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

  virtual void GetInfo(TSkinInfo* info) = 0;
  virtual QPixmapPtr GetImage(QChar ch, qreal zoom, bool cache) = 0;
};

typedef QSharedPointer<IClockSkin> ClockSkinPtr;

// TODO: move other file
SKIN_DRAWSHARED_EXPORT ClockSkinPtr CreateSkin(const QDir& skin_root);
SKIN_DRAWSHARED_EXPORT ClockSkinPtr CreateSkin(const QFont& font);

#endif // ICLOCK_SKIN_H
