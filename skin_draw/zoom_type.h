#ifndef SKIN_DRAW_ZOOM_TYPE_H
#define SKIN_DRAW_ZOOM_TYPE_H

#include <QMetaType>
#include <QSize>

namespace skin_draw {

struct Zoom
{
    qreal zoom_x;
    qreal zoom_y;

    Q_CONSTEXPR Zoom(qreal x, qreal y) Q_DECL_NOEXCEPT : zoom_x(x), zoom_y(y) {}
    Q_CONSTEXPR Zoom(qreal z) Q_DECL_NOEXCEPT : Zoom(z, z) {}
    Q_CONSTEXPR Zoom() Q_DECL_NOEXCEPT : Zoom(0.0) {}

    Q_CONSTEXPR bool isValid() const Q_DECL_NOEXCEPT
    {
        return !qFuzzyIsNull(zoom_x) && zoom_x > 0.0 &&
               !qFuzzyIsNull(zoom_y) && zoom_y > 0.0;
    }
};

inline Q_CONSTEXPR bool operator ==(const Zoom& lhs, const Zoom& rhs) Q_DECL_NOEXCEPT
{
    return qFuzzyCompare(lhs.zoom_x, rhs.zoom_x) && qFuzzyCompare(lhs.zoom_y, rhs.zoom_y);
}

inline Q_CONSTEXPR bool operator !=(const Zoom& lhs, const Zoom& rhs) Q_DECL_NOEXCEPT
{
    return !(lhs == rhs);
}

inline Q_CONSTEXPR bool operator <(const Zoom& lhs, const Zoom& rhs) Q_DECL_NOEXCEPT
{
    return !qFuzzyCompare(lhs.zoom_x, rhs.zoom_x) ?
                lhs.zoom_x < rhs.zoom_x :
                lhs.zoom_y < rhs.zoom_y;
}

inline Q_CONSTEXPR bool operator >(const Zoom& lhs, const Zoom& rhs) Q_DECL_NOEXCEPT
{
    return rhs < lhs;
}

inline Q_CONSTEXPR bool operator <=(const Zoom& lhs, const Zoom& rhs) Q_DECL_NOEXCEPT
{
    return !(rhs < lhs);
}

inline Q_CONSTEXPR bool operator >=(const Zoom& lhs, const Zoom& rhs) Q_DECL_NOEXCEPT
{
    return !(lhs < rhs);
}

inline Q_CONSTEXPR QSize operator *(const QSize& sz, const Zoom& z) Q_DECL_NOEXCEPT
{
    return QSize(sz.width() * z.zoom_x, sz.height() * z.zoom_y);
}

inline Q_CONSTEXPR QSizeF operator *(const QSizeF& sz, const Zoom& z) Q_DECL_NOEXCEPT
{
    return QSizeF(sz.width() * z.zoom_x, sz.height() * z.zoom_y);
}

} // namespace skin_draw

Q_DECLARE_METATYPE(skin_draw::Zoom)

#endif // SKIN_DRAW_ZOOM_TYPE_H
