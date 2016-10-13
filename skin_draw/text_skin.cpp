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

#include "text_skin.h"

#include <QPainter>
#include <QFontMetrics>

namespace skin_draw {

TextSkin::TextSkin(const QFont& font) : font_(font)
{
}

ISkin::QPixmapPtr TextSkin::ResizeImage(QChar ch, qreal zoom)
{
  auto iter = char_map_.find(ch);
  QChar sch = iter != char_map_.end() ? *iter : ch;
  QFont new_font(font_);
  qreal ik = new_font.italic() ? 1.5 : 1.0;
  new_font.setPointSizeF(font_.pointSizeF() * zoom * GetDevicePixelRatio());
  QFontMetrics fm(new_font);
  int res_w = new_font.italic() ? ik * fm.boundingRect(QString(sch)).width() : fm.width(QString(sch));
  if (res_w == 0) res_w = ik * fm.width(sch);
  QPixmapPtr result(new QPixmap(res_w, fm.height()));
  QPainter painter(result.data());
  painter.setFont(new_font);
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(result->rect(), Qt::transparent);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  painter.drawText(result->rect(), Qt::AlignCenter, ch);
  painter.end();
  return result;
}

} // namespace skin_draw
