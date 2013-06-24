#ifndef ICLOCK_SKIN_H
#define ICLOCK_SKIN_H

#include <QMap>
#include <QString>
#include <QPixmap>

// skin info fields
enum FSkinInfo {
  SI_NAME,        // skin name
  SI_VERSION,     // skin version
  SI_AUTHOR,      // skin author
  SI_EMAIL,       // author's e-mail
  SI_COMMENT,     // skin description
  SI_TYPE         // skin type ('raster/vector')
};
// skin info type
typedef QMap<FSkinInfo, QString> TSkinInfo;

class IClockSkin {
public:
  virtual ~IClockSkin() {}

  virtual const TSkinInfo& GetInfo() const = 0;
  virtual QPixmap* GetImage(const QString& s, qreal zoom, bool cache) = 0;
};

#endif // ICLOCK_SKIN_H
