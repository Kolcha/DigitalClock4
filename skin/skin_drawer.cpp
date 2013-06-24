#include <QFile>
#include "skin_drawer.h"

SkinDrawer::SkinDrawer(QObject *parent)
  : QObject(parent) {
  result_ = 0;
  skin_ = 0;
  texture_ = new QPixmap(8, 8);
}

SkinDrawer::~SkinDrawer() {
  delete result_;
  delete skin_;
  delete texture_;
}

void SkinDrawer::LoadSkin(const QString& skin_root) {
  delete skin_;
  skin_ = CreateSkin(skin_root);
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
  texture_->fill(new_color);
  Redraw();
}

void SkinDrawer::SetTexture(const QString& filename) {
  if (!QFile::exists(filename)) return;
  texture_->load(filename);
  Redraw();
}

void SkinDrawer::SetTexturePerElement(bool set) {
  txd_per_elem_ = set;
  Redraw();
}

void SkinDrawer::Redraw() {
  emit DrawingFinished(result_);
}
