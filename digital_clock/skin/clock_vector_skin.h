#ifndef CLOCK_VECTOR_SKIN_H
#define CLOCK_VECTOR_SKIN_H

#include <QDir>
#include "vector_skin.h"
#include "clock_base_skin.h"

class ClockVectorSkin : public VectorSkin, public ClockBaseSkin {
public:
  ClockVectorSkin(const QDir& skin_root);

protected:
  void ProcSeparators();
};

#endif // CLOCK_VECTOR_SKIN_H
