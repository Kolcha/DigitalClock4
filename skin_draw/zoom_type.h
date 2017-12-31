/*
    Digital Clock: skin draw engine
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

#ifndef SKIN_DRAW_ZOOM_TYPE_H
#define SKIN_DRAW_ZOOM_TYPE_H

#include <QMetaType>
#include <QSize>

namespace skin_draw {

struct Zoom {
  qreal zoom_x;
  qreal zoom_y;

  Q_DECL_CONSTEXPR Zoom(qreal x, qreal y) Q_DECL_NOEXCEPT : zoom_x(x), zoom_y(y) {}
  Q_DECL_CONSTEXPR Zoom(qreal z) Q_DECL_NOEXCEPT : Zoom(z, z) {}
  Q_DECL_CONSTEXPR Zoom() Q_DECL_NOEXCEPT : Zoom(0.0) {}

  Q_DECL_CONSTEXPR bool isValid() const Q_DECL_NOEXCEPT
  {
    return !qFuzzyIsNull(zoom_x) && (zoom_x > 0.0) && !qFuzzyIsNull(zoom_y) && (zoom_y > 0.0);
  }
};

inline Q_DECL_CONSTEXPR bool operator ==(const Zoom& lhs, const Zoom& rhs) Q_DECL_NOEXCEPT {
  return qFuzzyCompare(lhs.zoom_x, rhs.zoom_x) && qFuzzyCompare(lhs.zoom_y, rhs.zoom_y);
}

inline Q_DECL_CONSTEXPR bool operator !=(const Zoom& lhs, const Zoom& rhs) Q_DECL_NOEXCEPT {
  return !(lhs == rhs);
}

inline Q_DECL_CONSTEXPR bool operator <(const Zoom& lhs, const Zoom& rhs) Q_DECL_NOEXCEPT {
  return !qFuzzyCompare(lhs.zoom_x, rhs.zoom_x) ? lhs.zoom_x < rhs.zoom_x : lhs.zoom_y < rhs.zoom_y;
}

inline Q_DECL_CONSTEXPR bool operator >(const Zoom& lhs, const Zoom& rhs) Q_DECL_NOEXCEPT {
  return rhs < lhs;
}

inline Q_DECL_CONSTEXPR bool operator <=(const Zoom& lhs, const Zoom& rhs) Q_DECL_NOEXCEPT {
  return !(rhs < lhs);
}

inline Q_DECL_CONSTEXPR bool operator >=(const Zoom& lhs, const Zoom& rhs) Q_DECL_NOEXCEPT {
  return !(lhs < rhs);
}

inline Q_DECL_CONSTEXPR QSize operator *(const QSize& sz, const Zoom& z) Q_DECL_NOEXCEPT {
  return QSize(sz.width() * z.zoom_x, sz.height() * z.zoom_y);
}

inline Q_DECL_CONSTEXPR QSizeF operator *(const QSizeF& sz, const Zoom& z) Q_DECL_NOEXCEPT {
  return QSizeF(sz.width() * z.zoom_x, sz.height() * z.zoom_y);
}

} // namespace skin_draw

Q_DECLARE_METATYPE(skin_draw::Zoom)

#endif // SKIN_DRAW_ZOOM_TYPE_H
