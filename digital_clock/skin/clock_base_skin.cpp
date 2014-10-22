#include "clock_base_skin.h"

namespace digital_clock {
namespace core {

const BaseSkin::TSkinInfo& BaseSkin::GetInfo() const {
  return info_;
}

void BaseSkin::SetSeparators(const QString& seps) {
  seps_ = seps;
  seps_.remove(' ');
  ProcSeparators();
}

} // namespace core
} // namespace digital_clock
