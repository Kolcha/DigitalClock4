/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2017-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "card_layout.h"

namespace digital_clock {
namespace gui {

CardLayout::CardLayout(QWidget* parent): QLayout(parent)
{
}

CardLayout::~CardLayout()
{
  QLayoutItem* item;
  while ((item = takeAt(0)))
    delete item;
}

void CardLayout::addItem(QLayoutItem* item)
{
  m_list.append(item);
}

QSize CardLayout::sizeHint() const
{
  QSize s(0,0);
  int n = m_list.count();
  if (n > 0)
    s = QSize(100,70);  // start with a nice default size
  int i = 0;
  while (i < n) {
    QLayoutItem* o = m_list.at(i);
    s = s.expandedTo(o->sizeHint());
    ++i;
  }
  return s + n*QSize(spacing(), spacing()) + 2*QSize(margin(), margin());
}

QSize CardLayout::minimumSize() const
{
  QSize s(0,0);
  int n = m_list.count();
  int i = 0;
  while (i < n) {
    QLayoutItem* o = m_list.at(i);
    s = s.expandedTo(o->minimumSize());
    ++i;
  }
  return s + n*QSize(spacing(), spacing()) + 2*QSize(margin(), margin());
}

void CardLayout::setGeometry(const QRect& r)
{
  QLayout::setGeometry(r);

  if (m_list.size() == 0)
    return;

  int w = r.width() - (m_list.count() - 1) * spacing() - 2 * margin();
  int h = r.height() - (m_list.count() - 1) * spacing() - 2 * margin();
  int i = 0;
  while (i < m_list.size()) {
    QLayoutItem* o = m_list.at(i);
    QRect geom(r.x() + i * spacing() + margin(), r.y() + i * spacing() + margin(), w, h);
    o->setGeometry(geom);
    ++i;
  }
}

QLayoutItem* CardLayout::itemAt(int idx) const
{
  return m_list.value(idx);
}

QLayoutItem* CardLayout::takeAt(int idx)
{
  return idx >= 0 && idx < m_list.size() ? m_list.takeAt(idx) : 0;
}

int CardLayout::count() const
{
  return m_list.size();
}

} // namespace gui
} // namespace digital_clock
