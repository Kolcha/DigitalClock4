/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef DIGITAL_CLOCK_CORE_BASE_SKIN_H
#define DIGITAL_CLOCK_CORE_BASE_SKIN_H

#include <QMap>
#include <QString>
#include <QSharedPointer>

namespace digital_clock {
namespace core {

class BaseSkin
{
public:
  enum FSkinInfo {
    SI_NAME,        // skin name
    SI_VERSION,     // skin version
    SI_AUTHOR,      // skin author
    SI_EMAIL,       // author's e-mail
    SI_COMMENT      // skin description
  };
  typedef QMap<FSkinInfo, QString> TSkinInfo;

  virtual ~BaseSkin() {}

  const TSkinInfo& GetInfo() const;
  void SetSeparators(const QString& seps);

protected:
  virtual void ProcSeparators() = 0;

  TSkinInfo info_;
  QString seps_;
};

typedef QSharedPointer<BaseSkin> ClockSkinPtr;

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_BASE_SKIN_H
