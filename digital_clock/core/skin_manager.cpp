#include "../skin/clock_raster_skin.h"
#include "../skin/clock_vector_skin.h"
#include "../skin/clock_text_skin.h"
#include "skin_manager.h"

IClockSkin::ClockSkinPtr CreateSkin(const QDir& skin_root) {
  QStringList images = skin_root.entryList(QStringList("*.svg"), QDir::Files);
  bool skinini = skin_root.exists("skin.ini");
  IClockSkin::ClockSkinPtr skin;
  if (!images.empty() && skinini) skin.reset(new ClockVectorSkin(skin_root));
  images = skin_root.entryList(QStringList("*.png"), QDir::Files);
  if (!images.empty() && skinini) skin.reset(new ClockRasterSkin(skin_root));
  return skin;
}

IClockSkin::ClockSkinPtr CreateSkin(const QFont& font) {
  return IClockSkin::ClockSkinPtr(new ClockTextSkin(font));
}


SkinManager::SkinManager(QObject* parent)
  : QObject(parent) {
}

void SkinManager::AddSkinDir(const QDir& dir) {
  skin_dirs_.append(dir);
}

void SkinManager::DelSkinDir(const QDir& dir) {
  skin_dirs_.removeOne(dir);
}

void SkinManager::ListSkins() {
  skins_.clear();
  for (auto& s_dir : skin_dirs_) {
    QStringList f_dirs = s_dir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs);
    for (auto& f_dir : f_dirs) {
      QDir skin_root(s_dir.filePath(f_dir));
      IClockSkin::ClockSkinPtr tmp = CreateSkin(skin_root);
      if (!tmp) continue;
      const IClockSkin::TSkinInfo& info = tmp->GetInfo();
      skins_[info[IClockSkin::SI_NAME]] = skin_root;
    }
  }
  emit SearchFinished(skins_.keys());
}

void SkinManager::LoadSkin(const QString& skin_name) {
  IClockSkin::ClockSkinPtr skin;
  if (skin_name == "Text Skin") {
    skin = CreateSkin(font_);
  } else {
    skin = CreateSkin(skins_[skin_name]);
  }
  emit SkinLoaded(skin);
  // get skin info
  IClockSkin::TSkinInfo info;
  if (skin) info = skin->GetInfo();
  emit SkinInfoLoaded(info);
}

void SkinManager::SetFont(const QFont& font) {
  font_ = font;
  LoadSkin("Text Skin");
}
