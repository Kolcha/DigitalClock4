#ifndef CLOCK_TEXT_SKIN_H
#define CLOCK_TEXT_SKIN_H

#include "text_skin.h"
#include "clock_base_skin.h"

class ClockTextSkin : public SKIN_DRAW_NAMESPACE::TextSkin, public ClockBaseSkin {
public:
  ClockTextSkin(const QFont& font);

protected:
  void ProcSeparators();
};

#endif // CLOCK_TEXT_SKIN_H
