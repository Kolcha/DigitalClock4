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

#ifndef DIGITAL_CLOCK_CORE_TEXT_SKIN_H
#define DIGITAL_CLOCK_CORE_TEXT_SKIN_H

#include "text_skin.h"
#include "skin/clock_base_skin.h"

namespace digital_clock {
namespace core {

class TextSkin : public BaseSkin, public ::skin_draw::TextSkin
{
public:
  explicit TextSkin(const QFont& font);

protected:
  void ProcSeparators() override;
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_TEXT_SKIN_H
