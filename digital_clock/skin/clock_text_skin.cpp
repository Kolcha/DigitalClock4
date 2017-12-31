/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "clock_text_skin.h"

#include <QFontMetrics>

namespace digital_clock {
namespace core {

TextSkin::TextSkin(const QFont& font) : ::skin_draw::TextSkin(font)
{
  char_map_[' '] = ':';
  char_map_[':'] = ':';
  // set skin info
  info_[SI_NAME] = "Text Skin";
  info_[SI_VERSION] = "1.0";
  info_[SI_AUTHOR] = "Nick Korotysh";
  info_[SI_EMAIL] = "nick.korotysh@gmail.com";
  info_[SI_COMMENT] = "Text Skin. Allows use any font as skin.";
}

void TextSkin::ProcSeparators()
{
  if (seps_.isEmpty()) return;

  QFontMetrics fm(font_);
  int max_w = 0;
  QChar widest = seps_[0];
  for (int i = 0; i < seps_.length(); ++i) {
    int w = fm.width(QString(seps_[i]));
    if (w > max_w) {
      max_w = w;
      widest = seps_[i];
    }
  }

  img_cache_->Clear();

  char_map_.clear();
  char_map_[' '] = widest;
  for (int i = 0; i < seps_.length(); ++i) {
    char_map_[seps_[i]] = widest;
  }
}

} // namespace core
} // namespace digital_clock
