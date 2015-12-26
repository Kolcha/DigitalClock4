#ifndef DIGITAL_CLOCK_GUI_COLORIZE_EFFECT_H
#define DIGITAL_CLOCK_GUI_COLORIZE_EFFECT_H

#include <QImage>

namespace digital_clock {
namespace gui {

void colorize_image(const QImage& srcImage, QImage& destImage, const QColor& color, qreal strength);

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_COLORIZE_EFFECT_H
