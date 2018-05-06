/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "hover_buttons.h"

#include <QMouseEvent>
#include <QPaintEvent>

#include <QPainter>

namespace digital_clock {
namespace gui {

HoverButtons::HoverButtons(QWidget* parent)
  : QWidget(parent)
  , m_highlightColorBase(0, 170, 255, 128)
  , m_highlightColor(m_highlightColorBase)
{
  setMouseTracking(true);
}

void HoverButtons::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton && m_highlightRect.contains(event->pos())) {
    m_highlightColor = m_highlightColorBase.lighter();
    update();
    event->accept();
  } else {
    event->ignore();
  }
}

void HoverButtons::mouseReleaseEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton && m_highlightRect.contains(event->pos())) {
    m_highlightColor = m_highlightColorBase;
    update();
    emit buttonClicked(m_direction);
    event->accept();
  } else {
    event->ignore();
  }
}

void HoverButtons::mouseMoveEvent(QMouseEvent* event)
{
  const qreal button_size = qMin(width(), height()) / 3.;
  QRectF new_highlight;
  if (event->pos().x() <= button_size) {
    new_highlight = QRectF(0.5, 0.5, button_size, height() - 1);
    m_direction = Left;
  } else if (event->pos().x() >= width() - button_size) {
    new_highlight = QRectF(width() - button_size - 0.5, 0.5, button_size, height() - 1);
    m_direction = Right;
  } else if (event->pos().y() <= button_size) {
    new_highlight = QRectF(0.5, 0.5, width() - 1, button_size);
    m_direction = Top;
  } else if (event->pos().y() >= height() - button_size) {
    new_highlight = QRectF(0.5, height() - button_size - 0.5, width() - 1, button_size);
    m_direction = Bottom;
  } else {
    new_highlight = QRectF();
    m_direction = None;
  }

  if (m_highlightRect != new_highlight) {
    m_highlightRect = new_highlight;
    update();
  }

  event->setAccepted(m_highlightRect.contains(event->pos()));
}

void HoverButtons::leaveEvent(QEvent* event)
{
  if (!m_highlightRect.isEmpty()) {
    m_highlightRect = QRectF();
    m_direction = None;
    update();
    event->accept();
  } else {
    event->ignore();
  }
}

void HoverButtons::resizeEvent(QResizeEvent* event)
{
  m_highlightRect = QRectF();
  m_direction = None;
  QWidget::resizeEvent(event);
}

void HoverButtons::paintEvent(QPaintEvent* event)
{
  if (!m_highlightRect.size().isEmpty()) {
    QPainter p(this);
    p.setPen(m_highlightColorBase.darker());
    p.setBrush(m_highlightColor);
    p.drawRect(m_highlightRect);

    p.setPen(QPen(QColor(255, 255, 255, 128), 2));
    p.setRenderHint(QPainter::Antialiasing);

    QPointF c = m_highlightRect.center();
    qreal l = qMin(m_highlightRect.width(), m_highlightRect.height()) / 5.;

    switch (m_direction) {
      case Left:
        p.drawLine(c + QPointF(-0.5*l, 0), c + QPointF(+0.5*l, +l));
        p.drawLine(c + QPointF(-0.5*l, 0), c + QPointF(+0.5*l, -l));
        break;

      case Top:
        p.drawLine(c + QPointF(0, -0.5*l), c + QPointF(+l, +0.5*l));
        p.drawLine(c + QPointF(0, -0.5*l), c + QPointF(-l, +0.5*l));
        break;

      case Right:
        p.drawLine(c + QPointF(+0.5*l, 0), c + QPointF(-0.5*l, +l));
        p.drawLine(c + QPointF(+0.5*l, 0), c + QPointF(-0.5*l, -l));
        break;

      case Bottom:
        p.drawLine(c + QPointF(0, +0.5*l), c + QPointF(+l, -0.5*l));
        p.drawLine(c + QPointF(0, +0.5*l), c + QPointF(-l, -0.5*l));
        break;

      default:
        break;
    }
  }
  event->accept();
}

} // namespace gui
} // namespace digital_clock
