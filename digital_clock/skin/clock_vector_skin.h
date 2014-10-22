#ifndef DIGITAL_CLOCK_CORE_VECTOR_SKIN_H
#define DIGITAL_CLOCK_CORE_VECTOR_SKIN_H

#include <QDir>
#include "vector_skin.h"
#include "clock_base_skin.h"

namespace digital_clock {
namespace core {

class VectorSkin : public ::skin_draw::VectorSkin, public BaseSkin {
public:
  VectorSkin(const QDir& skin_root);

protected:
  void ProcSeparators();
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_VECTOR_SKIN_H
