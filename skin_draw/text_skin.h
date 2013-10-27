#ifndef TEXT_SKIN_H
#define TEXT_SKIN_H

#include <QFont>
#include "base_skin.h"

/*!
 * @brief The TextSkin class.
 *
 * TextSkin class implements logic to create skin object from any font.
 */
class SKIN_DRAWSHARED_EXPORT TextSkin : public BaseSkin {
public:
  /*!
   * Constructor.
   * @param font - font to use
   */
  explicit TextSkin(const QFont& font);

protected:
  /*!
   * Resize image for given character.
   * @param ch - requested charater
   * @param zoom - requested zoom
   * @return pointer to resized image
   */
  QPixmapPtr ResizeImage(QChar ch, qreal zoom);
  /// map with chars that need to be replaced
  QMap<QChar, QChar> char_map_;
  /// used font
  QFont font_;
};

#endif // TEXT_SKIN_H
