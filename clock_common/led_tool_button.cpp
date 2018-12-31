/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2016-2019  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "led_tool_button.h"

#include <QImage>
#include <QPainter>

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
