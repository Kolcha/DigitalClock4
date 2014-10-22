#ifndef DIGITAL_CLOCK_CORE_RASTER_SKIN_H
#define DIGITAL_CLOCK_CORE_RASTER_SKIN_H

#include <QDir>
#include "raster_skin.h"
#include "clock_base_skin.h"

namespace digital_clock {
namespace core {

class RasterSkin : public ::skin_draw::RasterSkin, public BaseSkin {
public:
  RasterSkin(const QDir& skin_root);

protected:
  void ProcSeparators();
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_RASTER_SKIN_H
