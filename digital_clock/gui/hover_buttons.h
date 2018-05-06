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

#ifndef DIGITAL_CLOCK_GUI_HOVER_BUTTONS_H
#define DIGITAL_CLOCK_GUI_HOVER_BUTTONS_H

#include <QWidget>

#include <QColor>
#include <QRectF>

namespace digital_clock {
namespace gui {

class HoverButtons : public QWidget
{
  Q_OBJECT

public:
  enum Direction {
    None,
    Left,
    Top,
    Right,
    Bottom
  };
  Q_ENUM(Direction)

  explicit HoverButtons(QWidget* parent = nullptr);

signals:
  void buttonClicked(Direction d);

public slots:

protected:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void leaveEvent(QEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

private:
  QColor m_highlightColorBase;
  Direction m_direction = None;
  QRectF m_highlightRect;
  QColor m_highlightColor;
};

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_HOVER_BUTTONS_H
