/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "clock_vector_skin.h"

#include <QSettings>

namespace digital_clock {
namespace core {

VectorSkin::VectorSkin(const QDir& skin_root)
{
  QSettings config(skin_root.filePath("skin.ini"), QSettings::IniFormat);
  // load info
  info_[SI_NAME] = config.value("info/name").toString();
  info_[SI_VERSION] = config.value("info/version").toString();
  info_[SI_AUTHOR] = config.value("info/author").toString();
  info_[SI_EMAIL] = config.value("info/e-mail").toString();
  info_[SI_COMMENT] = config.value("info/comment").toString();
  // load image files
  for (int i = 0; i < 10; ++i) {
    QString key = QString::number(i);
    image_files_[key[0]] = skin_root.filePath(config.value("files/" + key, "invalid").toString());
  }
  image_files_[':'] = skin_root.filePath(config.value("files/s1", "invalid").toString());
  image_files_[' '] = skin_root.filePath(config.value("files/s2", "invalid").toString());
  image_files_['a'] = skin_root.filePath(config.value("files/am", "invalid").toString());
  image_files_['p'] = skin_root.filePath(config.value("files/pm", "invalid").toString());
  image_files_['A'] = image_files_['a'];
  image_files_['P'] = image_files_['p'];
}

void VectorSkin::ProcSeparators()
{
  for (int i = 0; i < seps_.length(); ++i) {
    if (image_files_.find(seps_[i]) == image_files_.end()) {
      image_files_[seps_[i]] = image_files_[':'];
    }
  }
}

} // namespace core
} // namespace digital_clock
