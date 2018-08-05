/*
    Digital Clock - beautiful customizable clock with plugins
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

#include "skin_manager.h"

#include <QDir>
#include <QApplication>

#include "skin/clock_raster_skin.h"
#include "skin/clock_vector_skin.h"
#include "skin/clock_text_skin.h"

namespace digital_clock {
namespace core {

ClockSkinPtr CreateSkin(const QDir& skin_root)
{
  QStringList images = skin_root.entryList(QStringList("*.svg"), QDir::Files);
  bool skinini = skin_root.exists("skin.ini");
  ClockSkinPtr skin;
  if (!images.empty() && skinini) skin.reset(new VectorSkin(skin_root));
  images = skin_root.entryList(QStringList("*.png"), QDir::Files);
  if (!images.empty() && skinini) skin.reset(new RasterSkin(skin_root));
  return skin;
}

ClockSkinPtr CreateSkin(const QFont& font)
{
  return ClockSkinPtr(new TextSkin(font));
}


SkinManager::SkinManager(QObject* parent) : QObject(parent)
{
  search_paths_.append(":/clock/default_skins");
#ifdef Q_OS_MACOS
  search_paths_.append(qApp->applicationDirPath() + "/../Resources/skins");
#else
  search_paths_.append(qApp->applicationDirPath() + "/skins");
#endif
#ifdef Q_OS_LINUX
  search_paths_.append("/usr/share/digital_clock/skins");
  search_paths_.append("/usr/local/share/digital_clock/skins");
  search_paths_.append(QDir::homePath() + "/.local/share/digital_clock/skins");
#endif
}

ClockSkinPtr SkinManager::CurrentSkin() const
{
  return current_skin_;
}

void SkinManager::ListSkins()
{
  skins_.clear();
  for (auto& s_path : qAsConst(search_paths_)) {
    QDir s_dir(s_path);
    for (auto& f_dir : s_dir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs)) {
      QDir skin_root(s_dir.filePath(f_dir));
      ClockSkinPtr tmp = CreateSkin(skin_root);
      if (!tmp) continue;
      const BaseSkin::TSkinInfo& info = tmp->GetInfo();
      skins_[info[BaseSkin::SI_NAME]] = skin_root;
    }
  }
  emit SearchFinished(skins_.keys());
}

void SkinManager::LoadSkin(const QString& skin_name)
{
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
  current_skin_ = skin;
  SetSeparators(seps_);
  emit SkinLoaded(skin.dynamicCast<skin_draw::ISkin>());
  // get skin info
  BaseSkin::TSkinInfo info;
  if (skin) info = skin->GetInfo();
  emit SkinInfoLoaded(info);
}

void SkinManager::SetFont(const QFont& font)
{
  font_ = font;
  // update text skin if needed
  if (current_skin_.dynamicCast<TextSkin>()) LoadSkin("Text Skin");
}

void SkinManager::SetSeparators(const QString& seps)
{
  seps_ = seps;
  if (current_skin_) current_skin_->SetSeparators(seps);
}

void SkinManager::SetFallbackSkin(const QString& skin_name)
{
  fallback_skin_ = skin_name;
}

} // namespace core
} // namespace digital_clock
