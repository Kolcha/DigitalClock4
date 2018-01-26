/*
    Digital Clock: skin draw engine
    Copyright (C) 2013-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "raster_skin.h"

namespace skin_draw {

QPixmap RasterSkin::ResizeImage(const QString& str, int idx, Zoom zoom)
{
  const QPixmap& original = orig_images_[str[idx]];
  if (original.isNull()) return QPixmap();

  return original.scaled(original.size() * zoom,
                         Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

} // namespace skin_draw
