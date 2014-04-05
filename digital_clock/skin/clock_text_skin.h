#ifndef DIGITAL_CLOCK_CORE_CLOCK_TEXT_SKIN_H
#define DIGITAL_CLOCK_CORE_CLOCK_TEXT_SKIN_H

#include "text_skin.h"
#include "clock_base_skin.h"

namespace digital_clock {
namespace core {

class ClockTextSkin : public ::skin_draw::TextSkin, public ClockBaseSkin {
public:
  ClockTextSkin(const QFont& font);

protected:
  void ProcSeparators();
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_CLOCK_TEXT_SKIN_H
