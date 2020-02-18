/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2020  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef DIGITAL_CLOCK_CORE_VECTOR_SKIN_H
#define DIGITAL_CLOCK_CORE_VECTOR_SKIN_H

#include "vector_skin.h"
#include "skin/clock_base_skin.h"

#include <QDir>

namespace digital_clock {
namespace core {

class VectorSkin : public BaseSkin, public ::skin_draw::VectorSkin
{
public:
  explicit VectorSkin(const QDir& skin_root);

protected:
  void ProcSeparators() override;
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_VECTOR_SKIN_H
