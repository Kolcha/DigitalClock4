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

#ifndef SKIN_DRAW_ZOOM_TYPE_HPP
#define SKIN_DRAW_ZOOM_TYPE_HPP

#include <QMetaType>
#include <QSize>

namespace skin_draw {

class Zoom Q_DECL_FINAL {
public:
  Q_DECL_CONSTEXPR Zoom(qreal x, qreal y) Q_DECL_NOEXCEPT : zx_{x}, zy_{y} {}
  Q_DECL_CONSTEXPR Zoom(qreal z) Q_DECL_NOEXCEPT : Zoom{z, z} {}
  Q_DECL_CONSTEXPR Zoom() Q_DECL_NOEXCEPT : Zoom{0.0} {}

  Q_DECL_CONSTEXPR qreal zoomX() const Q_DECL_NOEXCEPT { return zx_; }
  Q_DECL_CONSTEXPR qreal zoomY() const Q_DECL_NOEXCEPT { return zy_; }

  void setZoomX(qreal x) Q_DECL_NOEXCEPT { zx_ = x; }
  void setZoomY(qreal y) Q_DECL_NOEXCEPT { zy_ = y; }

  Q_DECL_CONSTEXPR bool isValid() const Q_DECL_NOEXCEPT
  {
    return !qFuzzyIsNull(zx_) && (zx_ > 0.0) && !qFuzzyIsNull(zy_) && (zy_ > 0.0);
  }

private:
  qreal zx_;
  qreal zy_;
};

Q_DECL_CONSTEXPR inline bool operator ==(const Zoom& lhs, const Zoom& rhs) Q_DECL_NOEXCEPT
{
  return qFuzzyCompare(lhs.zoomX(), rhs.zoomX()) && qFuzzyCompare(lhs.zoomY(), rhs.zoomY());
}

Q_DECL_CONSTEXPR inline bool operator !=(const Zoom& lhs, const Zoom& rhs) Q_DECL_NOEXCEPT
{
  return !(lhs == rhs);
}

Q_DECL_CONSTEXPR inline bool operator <(const Zoom& lhs, const Zoom& rhs) Q_DECL_NOEXCEPT
{
  return !qFuzzyCompare(lhs.zoomX(), rhs.zoomX()) ? lhs.zoomX() < rhs.zoomX() : lhs.zoomY() < rhs.zoomY();
}

Q_DECL_CONSTEXPR inline bool operator >(const Zoom& lhs, const Zoom& rhs) Q_DECL_NOEXCEPT
{
  return rhs < lhs;
}

Q_DECL_CONSTEXPR inline bool operator <=(const Zoom& lhs, const Zoom& rhs) Q_DECL_NOEXCEPT
{
  return !(rhs < lhs);
}

Q_DECL_CONSTEXPR inline bool operator >=(const Zoom& lhs, const Zoom& rhs) Q_DECL_NOEXCEPT
{
  return !(lhs < rhs);
}

Q_DECL_CONSTEXPR inline QSize operator *(const QSize& sz, const Zoom& z) Q_DECL_NOEXCEPT
{
  return QSize(qRound(sz.width() * z.zoomX()), qRound(sz.height() * z.zoomY()));
}

Q_DECL_CONSTEXPR inline QSizeF operator *(const QSizeF& sz, const Zoom& z) Q_DECL_NOEXCEPT
{
  return QSizeF(sz.width() * z.zoomX(), sz.height() * z.zoomY());
}

Q_DECL_CONSTEXPR inline Zoom operator *(const Zoom& z, const qreal k) Q_DECL_NOEXCEPT
{
  return Zoom(z.zoomX() * k, z.zoomY() * k);
}

} // namespace skin_draw

Q_DECLARE_METATYPE(skin_draw::Zoom)

#endif // SKIN_DRAW_ZOOM_TYPE_HPP
