#ifndef TEXT_SKIN_H
#define TEXT_SKIN_H

#include "base_skin.h"

class TextSkin : public BaseSkin {
public:
  explicit TextSkin(const QFont& font);

protected:
  QPixmapPtr ResizeImage(const QString& s, qreal zoom);

private:
  QFont font_;
};

#endif // TEXT_SKIN_H
