#ifndef DIGITAL_CLOCK_CORE_CLOCK_VECTOR_SKIN_H
#define DIGITAL_CLOCK_CORE_CLOCK_VECTOR_SKIN_H

#include <QDir>
#include "vector_skin.h"
#include "clock_base_skin.h"

namespace digital_clock {
namespace core {

class ClockVectorSkin : public ::skin_draw::VectorSkin, public ClockBaseSkin {
public:
  ClockVectorSkin(const QDir& skin_root);

protected:
  void ProcSeparators();
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_CLOCK_VECTOR_SKIN_H
