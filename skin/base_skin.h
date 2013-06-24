#ifndef BASE_SKIN_H
#define BASE_SKIN_H

#include <QStringList>
#include "iclock_skin.h"

class BaseSkin : public IClockSkin {
public:
  BaseSkin(const QString& skin_root);
  virtual ~BaseSkin();

  const TSkinInfo& GetInfo() const;

protected:
  QStringList image_files_;
  QMap<QString, QPixmap*> image_cache_;

private:
  void GenImgKeys();
  void LoadConfig();
  TSkinInfo info_;
  QStringList image_keys_;
  QString skin_root_;
};

#endif // BASE_SKIN_H
