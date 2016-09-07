#ifndef DIGITAL_CLOCK_CORE_TEXT_SKIN_H
#define DIGITAL_CLOCK_CORE_TEXT_SKIN_H

#include "text_skin.h"
#include "skin/clock_base_skin.h"

namespace digital_clock {
namespace core {

class TextSkin : public ::skin_draw::TextSkin, public BaseSkin
{
public:
  TextSkin(const QFont& font);

protected:
  void ProcSeparators();
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_TEXT_SKIN_H
