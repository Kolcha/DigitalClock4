/*
    Digital Clock: skin draw engine
    Copyright (C) 2013-2016  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SKIN_DRAW_TEXT_SKIN_H
#define SKIN_DRAW_TEXT_SKIN_H

#include "base_skin.h"

#include <QMap>
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
class SKIN_DRAWSHARED_EXPORT TextSkin : public BaseSkin
{
public:
  /*!
   * Constructor.
   * @param font - font to use
   */
  explicit TextSkin(const QFont& font);

protected:
  /*!
   * Resize image for given character.
   * @param str - text to render
   * @param idx - symbol index
   * @param zoom - requested zoom
   * @return pointer to resized image
   */
  QPixmapPtr ResizeImage(const QString& str, int idx, qreal zoom);
  /// map with chars that need to be replaced (old - new char)
  QMap<QChar, QChar> char_map_;
  /// used font
  QFont font_;
};

} // namespace skin_draw

#endif // SKIN_DRAW_TEXT_SKIN_H
