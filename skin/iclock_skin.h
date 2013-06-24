#ifndef ICLOCK_SKIN_H
#define ICLOCK_SKIN_H

#include <QMap>
#include <QString>
#include <QPixmap>

class IClockSkin {
public:
  virtual ~IClockSkin() {}

  virtual const QMap<QString, QString>& GetSkinInfo() const = 0;
  virtual QPixmap* GetImage(QChar ch, qreal zoom, bool cache = true) = 0;
};

#endif // ICLOCK_SKIN_H
