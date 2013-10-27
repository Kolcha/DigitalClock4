#include "clock_base_skin.h"

ClockBaseSkin::ClockBaseSkin() {
}

ClockBaseSkin::~ClockBaseSkin() {
}

void ClockBaseSkin::SetSeparators(const QString& seps) {
  seps_ = seps;
  seps_.remove(' ');
  ProcSeparators();
}
