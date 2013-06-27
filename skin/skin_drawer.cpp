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
  texture_.load(filename);
  Redraw();
}

void SkinDrawer::SetTexturePerElement(bool set) {
  txd_per_elem_ = set;
  Redraw();
}

void SkinDrawer::SetPreviewMode(bool set) {
  preview_mode_ = set;
}

void SkinDrawer::Redraw() {
  if (str_.isEmpty() || !skin_) return;
  // get images for all symbols
  QList<QImage*> elements;
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
  QImage* result = new QImage(result_w, result_h, QImage::Format_ARGB32_Premultiplied);
  QPainter painter(result);
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(result->rect(), Qt::transparent);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

  if (txd_per_elem_) {
    int x = 0;
    for (auto& elem : elements) {
      // draw mask
      painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
      painter.drawImage(x, 0, *elem);
      // draw texture
      painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
      painter.drawTiledPixmap(elem->rect(), texture_, QPoint(x, 0));
      x += elem->width() + space;
    }
  } else {
    // draw mask
    int x = 0;
    for (auto& elem : elements) {
      painter.drawImage(x, 0, *elem);
      x += elem->width() + space;
    }
    // draw texture
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.drawTiledPixmap(result->rect(), texture_);
  }
  painter.end();

  emit DrawingFinished(result);
}
