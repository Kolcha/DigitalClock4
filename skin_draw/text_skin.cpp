#include <QPainter>
#include <QFontMetrics>
#include "text_skin.h"

TextSkin::TextSkin(const QFont& font) : font_(font) {
  // set skin info
  info_[SI_NAME] = "Text Skin";
  info_[SI_VERSION] = "1.0";
  info_[SI_AUTHOR] = "Nick Korotysh";
  info_[SI_EMAIL] = "nick.korotysh@gmail.com";
  info_[SI_COMMENT] = "Text Skin. Allows use any font as skin.";
}

QPixmapPtr TextSkin::ResizeImage(const QString& s, qreal zoom) {
  QString ls = s == " " ? ":" : s;
  QFont new_font(font_);
  new_font.setPointSizeF(font_.pointSizeF() * zoom);
  QFontMetrics fm(new_font);
  QPixmapPtr result(new QPixmap(fm.width(ls), fm.height()));
  QPainter painter(result.data());
  painter.setFont(new_font);
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(result->rect(), Qt::transparent);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  painter.drawText(result->rect(), Qt::AlignCenter, s);
  return result;
}

void TextSkin::CharToKey(QChar ch, QString& s) {
  s = QString(ch);
}
