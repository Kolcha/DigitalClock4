#include <QFile>
#include <QList>
#include <QPainter>
#include "skin_drawer.h"

SkinDrawer::SkinDrawer(QObject* parent)
  : QObject(parent), texture_(8, 8) {
  skin_ = 0;
  zoom_ = 1.0;
  txd_per_elem_ = false;
  preview_mode_ = false;
}

SkinDrawer::~SkinDrawer() {
  delete skin_;
}

void SkinDrawer::LoadSkin(const QDir& skin_root) {
  delete skin_;
  skin_ = CreateSkin(skin_root);
  if (!skin_) return;
  emit LoadedSkinInfo(skin_->GetInfo());
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

void SkinDrawer::SetColor(const QColor& new_color) {
  if (!new_color.isValid()) return;
  texture_.fill(new_color);
  Redraw();
}

void SkinDrawer::SetTexture(const QString& filename) {
  if (!QFile::exists(filename)) return;
  if (!use_txd_) {
    txd_file_ = filename;
    return;
  }
  texture_.load(filename);
  Redraw();
}

void SkinDrawer::SetTexturePerElement(bool set) {
  txd_per_elem_ = set;
  Redraw();
}

void SkinDrawer::SetTextureDrawMode(SkinDrawer::DrawMode mode) {
  txd_draw_mode_ = mode;
  Redraw();
}

void SkinDrawer::SetUseTexture(bool set) {
  use_txd_ = set;
  SetTexture(txd_file_);
}

void SkinDrawer::SetPreviewMode(bool set) {
  preview_mode_ = set;
}

void SkinDrawer::Redraw() {
  if (str_.isEmpty() || !skin_) return;
  // get images for all symbols
  QList<QPixmap*> elements;
  for (auto i = str_.begin(); i != str_.end(); ++i) {
    elements.push_back(skin_->GetImage(*i, zoom_, !preview_mode_));
  }
  // calculate result image width and height
  int result_w = 0;
  int result_h = 0;
  for (auto& elem : elements) {
    result_w += elem->width();
    result_h = qMax(result_h, elem->height());
  }
  // leave some space between images
  int space = 4;
  result_w += space * (str_.length() - 1);

  // create result image
  QPixmap result(result_w, result_h);
  QPainter painter(&result);
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(result.rect(), Qt::transparent);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

  int x = 0;
  for (auto& elem : elements) {
    // draw mask
    painter.drawPixmap(x, 0, *elem);
    if (txd_per_elem_) {
      // draw texture
      DrawTexture(painter, QRect(x, 0, elem->width(), elem->height()));
    }
    x += elem->width() + space;
  }
  if (!txd_per_elem_) {
    // draw texture
    DrawTexture(painter, result.rect());
  }
  painter.end();

  emit DrawingFinished(result);
  if (preview_mode_) {
    for (auto& elem : elements) {
      delete elem;
    }
  }
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
