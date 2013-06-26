#ifndef VECTOR_SKIN_H
#define VECTOR_SKIN_H

#include "base_skin.h"

class VectorSkin : public BaseSkin {
public:
  explicit VectorSkin(const QDir& skin_root);

protected:
  const QImage& ResizeImage(const QString& s, qreal zoom);
};

#endif // VECTOR_SKIN_H
