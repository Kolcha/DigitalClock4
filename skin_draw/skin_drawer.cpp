#include "skin_drawer.h"

#include <QGuiApplication>
#include <QScreen>
#include <QFile>
#include <QList>
#include <QPainter>

namespace skin_draw {

SkinDrawer::SkinDrawer(QObject* parent)
  : QObject(parent), texture_(8, 8) {
  device_pixel_ratio_ = QGuiApplication::primaryScreen()->devicePixelRatio();
  zoom_ = 1.0;
  txd_per_elem_ = false;
  preview_mode_ = false;
  cust_type_ = CT_COLOR;
  space_ = 4 * device_pixel_ratio_;
}

void SkinDrawer::ApplySkin(ISkin::SkinPtr skin) {
  skin_ = skin;
  Redraw();
}

void SkinDrawer::SetString(const QString& str) {
  str_ = str;
  Redraw();
}

void SkinDrawer::SetZoom(qreal new_zoom) {
  zoom_ = new_zoom;
  Redraw();
}

bool SkinDrawer::SetColor(const QColor& new_color) {
  if (!new_color.isValid()) return false;
  color_ = new_color;
  if (cust_type_ != CT_COLOR) return false;
  texture_.fill(new_color);
  Redraw();
  return true;
}

bool SkinDrawer::SetTexture(const QString& filename) {
  if (!QFile::exists(filename)) return false;
  txd_file_ = filename;
  if (cust_type_ != CT_TEXTURE) return false;
  texture_.load(filename);
  Redraw();
  return true;
}

void SkinDrawer::SetTexturePerElement(bool set) {
  txd_per_elem_ = set;
  Redraw();
}

void SkinDrawer::SetTextureDrawMode(SkinDrawer::DrawMode mode) {
  txd_draw_mode_ = mode;
  Redraw();
}

void SkinDrawer::SetCustomizationType(CustomizationType type) {
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

void SkinDrawer::SetSpace(int new_space) {
  space_ = new_space * device_pixel_ratio_;
  Redraw();
}

void SkinDrawer::SetPreviewMode(bool set) {
  preview_mode_ = set;
}

void SkinDrawer::Redraw() {
  if (str_.isEmpty() || !skin_) return;
  // get images for all symbols
  QList<ISkin::QPixmapPtr> elements;
  for (auto i = str_.begin(); i != str_.end(); ++i) {
    elements.push_back(skin_->GetImage(*i, zoom_, !preview_mode_));
  }
  // calculate result image width and height
  int result_w = 0;
  int result_h = 0;
  int c_row_w = 0;
  int c_row_n = 0;
  int m_row_n = 0;
  for (auto& elem : elements) {
    ++c_row_n;
    if (!elem) continue;
    if (elem->size() == QSize(0,0)) {
      if (c_row_w >= result_w) {
        result_w = c_row_w;
        m_row_n = c_row_n;
      }
      c_row_w = 0;
      c_row_n = 0;
      continue;
    }
    c_row_w += elem->width();
    result_h = qMax(result_h, elem->height());
  }
  if (c_row_w >= result_w) {
    result_w = c_row_w;
    m_row_n = c_row_n;
  }
  int elem_h = result_h;
  // leave some space between images
  result_w += space_ * (m_row_n - 1);
  int rows = str_.count('\n');
  result_h = result_h * (rows + 1) + space_ * rows;

  // create result image
  QImage result(result_w, result_h, QImage::Format_ARGB32_Premultiplied);
  result.setDevicePixelRatio(device_pixel_ratio_);
  QPainter painter(&result);
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(result.rect(), Qt::transparent);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

  int x = 0;
  int y = 0;
  for (auto& elem : elements) {
    // draw mask
    if (!elem) continue;
    if (elem->size() == QSize(0,0)) {
      x = 0;
      y += (elem_h + space_) / elem->devicePixelRatio();
      continue;
    }
    painter.drawPixmap(x, y, *elem);
    if (txd_per_elem_ && cust_type_ != CT_NONE) {
      // draw texture
      DrawTexture(painter, QRect(x, y, elem->width(), elem->height()));
    }
    x += (elem->width() + space_) / elem->devicePixelRatio();
  }
  if (!txd_per_elem_ && cust_type_ != CT_NONE) {
    // draw texture
    DrawTexture(painter, result.rect());
  }
  painter.end();

  emit DrawingFinished(result);
}

void SkinDrawer::DrawTexture(QPainter& painter, const QRect& rect) {
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
