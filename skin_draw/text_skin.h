#ifndef TEXT_SKIN_H
#define TEXT_SKIN_H

#include "base_skin.h"
#include <QFont>

namespace skin_draw {

/*!
 * @brief Any font support class.
 *
 * TextSkin class implements logic to create skin object from any font.
 * This class is ready for use. You can use it as it implemented there or inherit it and
 * change something in their behaviour (implement symbols replace logic for example).
 * To replace some symblols you must fill TextSkin::char_map_ map.
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
  /// map with chars that need to be replaced (old - new char)
  QMap<QChar, QChar> char_map_;
  /// used font
  QFont font_;
};

} // namespace skin_draw

#endif // TEXT_SKIN_H
