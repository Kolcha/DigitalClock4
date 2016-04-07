#include "led_tool_button.h"

#include <QImage>
#include <QPainter>

namespace digital_clock {
namespace gui {

LedToolButton::LedToolButton(QWidget* parent) : QToolButton(parent)
{
  blockSignals(true);
  setColor(Qt::magenta);
  blockSignals(false);
}

QColor LedToolButton::color() const
{
  return color_;
}

void LedToolButton::setColor(const QColor& color)
{
  color_ = color;

  QImage img(64, 64, QImage::Format_ARGB32_Premultiplied);
  QPainter p(&img);
  p.setCompositionMode(QPainter::CompositionMode_Source);
  p.fillRect(img.rect(), Qt::transparent);
  p.setCompositionMode(QPainter::CompositionMode_SourceOver);
  p.setPen(color);
  p.setBrush(color);
  p.drawRoundedRect(0.0625 * img.width(), 0.75 * img.height(), 0.875 * img.width(), 0.25 * img.height(), 8, 8);
  p.end();

  setIcon(QIcon(QPixmap::fromImage(img)));

  emit colorChanged(color);
}

} // namespace gui
} // namespace digital_clock
