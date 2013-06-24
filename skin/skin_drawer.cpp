#include "skin_drawer.h"

SkinDrawer::SkinDrawer(QObject *parent)
  : QObject(parent) {
  skin_ = 0;
}

SkinDrawer::~SkinDrawer() {
  delete skin_;
}

QPixmap* SkinDrawer::DrawString(const QString& str) {
  return 0;
}

void SkinDrawer::LoadSkin(const QString& skin_root) {
  delete skin_;
  skin_ = CreateSkin(skin_root);
}
