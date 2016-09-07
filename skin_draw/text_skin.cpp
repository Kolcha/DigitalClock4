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
  new_font.setPointSizeF(font_.pointSizeF() * zoom * device_pixel_ratio_);
  QFontMetrics fm(new_font);
  int res_w = new_font.italic() ? ik * fm.boundingRect(QString(sch)).width() : fm.width(QString(sch));
  QPixmapPtr result(new QPixmap(res_w, fm.height()));
  QPainter painter(result.data());
  painter.setFont(new_font);
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(result->rect(), Qt::transparent);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  painter.drawText(result->rect(), Qt::AlignCenter, ch);
  painter.end();
  result->setDevicePixelRatio(device_pixel_ratio_);
  return result;
}

} // namespace skin_draw
