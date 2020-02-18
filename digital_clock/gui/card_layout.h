/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2017-2020  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef DIGITAL_CLOCK_GUI_CARD_LAYOUT_H
#define DIGITAL_CLOCK_GUI_CARD_LAYOUT_H

#include <QLayout>
#include <QList>

namespace digital_clock {
namespace gui {

class CardLayout : public QLayout
{
public:
  explicit CardLayout(QWidget* parent);
  ~CardLayout();

  void addItem(QLayoutItem* item);

  QSize sizeHint() const;
  QSize minimumSize() const;

  void setGeometry(const QRect& rect);

  QLayoutItem* itemAt(int idx) const;
  QLayoutItem* takeAt(int idx);

  int count() const;

private:
  QList<QLayoutItem*> m_list;
};

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_CARD_LAYOUT_H
