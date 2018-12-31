/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2019  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef DIGITAL_CLOCK_CORE_SKIN_MANAGER_H
#define DIGITAL_CLOCK_CORE_SKIN_MANAGER_H

#include <QObject>

#include <QString>
#include <QFont>
#include <QMap>
#include <QDir>

#include "iskin.h"
#include "skin/clock_base_skin.h"

namespace digital_clock {
namespace core {

class SkinManager : public QObject
{
  Q_OBJECT

public:
  explicit SkinManager(QObject* parent = nullptr);
  ClockSkinPtr CurrentSkin() const;

signals:
  void SearchFinished(const QStringList& skins);
  void SkinLoaded(skin_draw::ISkin::SkinPtr skin);
  void SkinInfoLoaded(const BaseSkin::TSkinInfo& info);
  void ErrorMessage(const QString& msg);

public slots:
  void ListSkins();
  void LoadSkin(const QString& skin_name);
  void SetFont(const QFont& font);
  void SetSeparators(const QString& seps);
  void SetFallbackSkin(const QString& skin_name);

private:
  QStringList search_paths_;
  QMap<QString, QDir> skins_;
  QFont font_;
  QString seps_;
  ClockSkinPtr current_skin_;
  QString fallback_skin_;
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_SKIN_MANAGER_H
