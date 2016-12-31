/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#include <QPainter>

namespace digital_clock {
namespace gui {

// grayscales the image to dest (could be same). If rect isn't defined
// destination image size is used to determine the dimension of grayscaling
// process.
static void grayscale(const QImage& image, QImage& dest, const QRect& rect = QRect())
{
  QRect destRect = rect;
  QRect srcRect = rect;
  if (rect.isNull()) {
    srcRect = dest.rect();
    destRect = dest.rect();
  }
  if (&image != &dest) {
    destRect.moveTo(QPoint(0, 0));
  }

  const unsigned int* data = (const unsigned int*)image.bits();
  unsigned int* outData = (unsigned int*)dest.bits();

  if (dest.size() == image.size() && image.rect() == srcRect) {
    // a bit faster loop for grayscaling everything
    int pixels = dest.width() * dest.height();
    for (int i = 0; i < pixels; ++i) {
      int val = qGray(data[i]);
      outData[i] = qRgba(val, val, val, qAlpha(data[i]));
    }
  } else {
    int yd = destRect.top();
    for (int y = srcRect.top(); y <= srcRect.bottom() && y < image.height(); y++) {
      data = (const unsigned int*)image.scanLine(y);
      outData = (unsigned int*)dest.scanLine(yd++);
      int xd = destRect.left();
      for (int x = srcRect.left(); x <= srcRect.right() && x < image.width(); x++) {
        int val = qGray(data[x]);
        outData[xd++] = qRgba(val, val, val, qAlpha(data[x]));
      }
    }
  }
}

// code based on QGraphicsColorizeEffect code, added Retina support
void colorize_image(const QImage& srcImage, QImage& destImage, const QColor& color, qreal strength)
{
  strength = qBound(qreal(0), strength, qreal(1));
  bool opaque = !qFuzzyIsNull(strength);
  bool alphaBlend = !qFuzzyIsNull(strength - 1);

  if (!opaque) {
    destImage = srcImage;
    return;
  }

  destImage = QImage(srcImage.size(), srcImage.format());
  destImage.setDevicePixelRatio(srcImage.devicePixelRatio());

  // do colorizing
  QPainter destPainter(&destImage);
  grayscale(srcImage, destImage, srcImage.rect());
  destPainter.setCompositionMode(QPainter::CompositionMode_Screen);
  destPainter.fillRect(srcImage.rect(), color);
  destPainter.end();

  if (alphaBlend) {
    // alpha blending srcImage and destImage
    QImage buffer = srcImage;
    QPainter bufPainter(&buffer);
    bufPainter.setOpacity(strength);
    bufPainter.drawImage(0, 0, destImage);
    bufPainter.end();
    destImage = buffer;
  }

  if (srcImage.hasAlphaChannel())
    destImage.setAlphaChannel(srcImage.alphaChannel());
}

} // namespace gui
} // namespace digital_clock
