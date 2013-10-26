#include <QFile>
#include <QList>
#include <QPainter>
#include "skin_drawer.h"

SkinDrawer::SkinDrawer(QObject* parent)
  : QObject(parent), texture_(8, 8) {
  zoom_ = 1.0;
  txd_per_elem_ = false;
  preview_mode_ = false;
  cust_type_ = CT_COLOR;
  space_ = 4;
}

void SkinDrawer::ApplySkin(ISkin::ClockSkinPtr skin) {
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
  space_ = new_space;
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
  for (auto& elem : elements) {
    if (!elem) continue;
    result_w += elem->width();
    result_h = qMax(result_h, elem->height());
  }
  // leave some space between images
  result_w += space_ * (str_.length() - 1);

  // create result image
  QImage result(result_w, result_h, QImage::Format_ARGB32_Premultiplied);
  QPainter painter(&result);
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(result.rect(), Qt::transparent);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

  int x = 0;
  for (auto& elem : elements) {
    // draw mask
    if (!elem) continue;
    painter.drawPixmap(x, 0, *elem);
    if (txd_per_elem_ && cust_type_ != CT_NONE) {
      // draw texture
      DrawTexture(painter, QRect(x, 0, elem->width(), elem->height()));
    }
    x += elem->width() + space_;
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
