#include "../skin/clock_raster_skin.h"
#include "../skin/clock_vector_skin.h"
#include "../skin/clock_text_skin.h"
#include "skin_manager.h"

namespace digital_clock {
namespace core {

ClockSkinPtr CreateSkin(const QDir& skin_root) {
  QStringList images = skin_root.entryList(QStringList("*.svg"), QDir::Files);
  bool skinini = skin_root.exists("skin.ini");
  ClockSkinPtr skin;
  if (!images.empty() && skinini) skin.reset(new ClockVectorSkin(skin_root));
  images = skin_root.entryList(QStringList("*.png"), QDir::Files);
  if (!images.empty() && skinini) skin.reset(new ClockRasterSkin(skin_root));
  return skin;
}

ClockSkinPtr CreateSkin(const QFont& font) {
  return ClockSkinPtr(new ClockTextSkin(font));
}


SkinManager::SkinManager(QObject* parent) : ManagerBase(parent), ratio_(1.0) {
}

void SkinManager::ListSkins() {
  skins_.clear();
  for (auto& s_dir : search_dirs_) {
    QStringList f_dirs = s_dir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs);
    for (auto& f_dir : f_dirs) {
      QDir skin_root(s_dir.filePath(f_dir));
      ClockSkinPtr tmp = CreateSkin(skin_root);
      if (!tmp) continue;
      const ClockBaseSkin::TSkinInfo& info = tmp->GetInfo();
      skins_[info[ClockBaseSkin::SI_NAME]] = skin_root;
    }
  }
  emit SearchFinished(skins_.keys());
}

void SkinManager::LoadSkin(const QString& skin_name) {
  ClockSkinPtr skin;
  if (skin_name == "Text Skin") {
    skin = CreateSkin(font_);
  } else {
    skin = CreateSkin(skins_[skin_name]);
  }
  if (!skin && !fallback_skin_.isEmpty()) {
    emit ErrorMessage(tr("Skin '%1' is not loaded, using default skin.").arg(skin_name));
    skin = CreateSkin(skins_[fallback_skin_]);
  }
  skin.dynamicCast<skin_draw::ISkin>()->SetDevicePixelRatio(ratio_);
  current_skin_ = skin;
  SetSeparators(seps_);
  emit SkinLoaded(skin.dynamicCast<skin_draw::ISkin>());
  // get skin info
  ClockBaseSkin::TSkinInfo info;
  if (skin) info = skin->GetInfo();
  emit SkinInfoLoaded(info);
}

void SkinManager::SetFont(const QFont& font) {
  font_ = font;
  // update text skin if needed
  if (current_skin_.dynamicCast<ClockTextSkin>()) LoadSkin("Text Skin");
}

void SkinManager::SetSeparators(const QString& seps) {
  seps_ = seps;
  if (current_skin_) current_skin_->SetSeparators(seps);
}

void SkinManager::SetFallbackSkin(const QString& skin_name) {
  fallback_skin_ = skin_name;
}

void SkinManager::SetDevicePixelRatio(qreal new_ratio) {
  ratio_ = new_ratio;
}

} // namespace core
} // namespace digital_clock
