#ifndef DIGITAL_CLOCK_CORE_CLOCK_RASTER_SKIN_H
#define DIGITAL_CLOCK_CORE_CLOCK_RASTER_SKIN_H

#include <QDir>
#include "raster_skin.h"
#include "clock_base_skin.h"

namespace digital_clock {
namespace core {

class ClockRasterSkin : public ::skin_draw::RasterSkin, public ClockBaseSkin {
public:
  ClockRasterSkin(const QDir& skin_root);

protected:
  void ProcSeparators();
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_CLOCK_RASTER_SKIN_H
