/*
    Digital Clock: skin draw engine
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

#include "skin_drawer.h"

#include <QFile>
#include <QList>
#include <QPainter>

namespace skin_draw {

SkinDrawer::SkinDrawer(QObject* parent) :
  QObject(parent),
  texture_(8, 8), txd_draw_mode_(DM_TILE),
  color_(Qt::magenta), cust_type_(CT_COLOR)
{
  zoom_ = 1.0;
  txd_per_elem_ = false;
  preview_mode_ = false;
  space_ = 4;
  texture_.fill(color_);
}

void SkinDrawer::ApplySkin(ISkin::SkinPtr skin)
{
  skin_ = skin;
  Redraw();
}

void SkinDrawer::SetString(const QString& str)
{
  str_ = str;
  Redraw();
}

void SkinDrawer::SetZoom(qreal new_zoom)
{
  zoom_ = new_zoom;
  Redraw();
}

bool SkinDrawer::SetColor(const QColor& new_color)
{
  if (!new_color.isValid()) return false;
  color_ = new_color;
  if (cust_type_ != CT_COLOR) return false;
  texture_.fill(new_color);
  Redraw();
  return true;
}

bool SkinDrawer::SetTexture(const QString& filename)
{
  if (!QFile::exists(filename)) return false;
  txd_file_ = filename;
  if (cust_type_ != CT_TEXTURE) return false;
  texture_.load(filename);
  Redraw();
  return true;
}

void SkinDrawer::SetTexturePerElement(bool set)
{
  txd_per_elem_ = set;
  Redraw();
}

void SkinDrawer::SetTextureDrawMode(SkinDrawer::DrawMode mode)
{
  txd_draw_mode_ = mode;
  Redraw();
}

void SkinDrawer::SetCustomizationType(CustomizationType type)
{
  cust_type_ = type;
  switch (type) {
    case CT_NONE:
      Redraw();
      break;

    case CT_COLOR:
      if (!SetColor(color_)) SetCustomizationType(CT_NONE);
      break;

    case CT_TEXTURE:
      if (!SetTexture(txd_file_)) SetCustomizationType(CT_COLOR);
      break;
  }
}

void SkinDrawer::SetSpace(int new_space)
{
  space_ = new_space;
  Redraw();
}

void SkinDrawer::SetPreviewMode(bool set)
{
  preview_mode_ = set;
}

void SkinDrawer::Redraw()
{
  if (str_.isEmpty() || !skin_) return;

  QList<QList<ISkin::QPixmapPtr> > elements;
  QList<ISkin::QPixmapPtr> row_elements;

  const int space = space_ * skin_->GetDevicePixelRatio();

  int result_w = 0;
  int result_h = 0;

  int cur_row_w = 0;
  int cur_row_h = 0;

  for (int i = 0; i < str_.length(); ++i) {
    if (str_[i] == '\n') {
      result_w = std::max(result_w, cur_row_w);
      result_h += cur_row_h + space;
      cur_row_w = 0;
      elements.append(row_elements);
      row_elements.clear();
      continue;
    }

    ISkin::QPixmapPtr elem = skin_->GetImage(str_, i, zoom_, !preview_mode_);
    if (!elem || elem->isNull()) continue;
    row_elements.append(elem);

    if (cur_row_w > 0) cur_row_w += space;
    cur_row_w += elem->width();
    cur_row_h = std::max(elem->height(), cur_row_h);
  }

  result_w = std::max(result_w, cur_row_w);
  result_h += cur_row_h;
  elements.append(row_elements);

  QImage result(result_w, result_h, QImage::Format_ARGB32_Premultiplied);
  {
    QPainter painter(&result);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(result.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    QPoint pt(0, 0);

    for (auto& row : elements) {
      pt.setX(0);
      int row_h = 0;

      for (auto& elem : row) {
        row_h = std::max(elem->height(), row_h);
        painter.drawPixmap(pt, *elem);

        if (txd_per_elem_ && cust_type_ != CT_NONE)
          DrawTexture(painter, QRect(pt, elem->size()));

        pt.rx() += elem->width() + space;
      }

      pt.ry() += row_h + space;
    }

    if (!txd_per_elem_ && cust_type_ != CT_NONE)
      DrawTexture(painter, result.rect());
  }
  result.setDevicePixelRatio(skin_->GetDevicePixelRatio());
  emit DrawingFinished(result);
}

void SkinDrawer::DrawTexture(QPainter& painter, const QRect& rect)
{
  QPainter::CompositionMode old_mode = painter.compositionMode();
  painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
  switch (txd_draw_mode_) {
    case DM_STRETCH:
      painter.drawPixmap(rect, texture_, texture_.rect());
      break;

    case DM_TILE:
      painter.drawTiledPixmap(rect, texture_);
      break;
  }
  painter.setCompositionMode(old_mode);
}

} // namespace skin_draw
