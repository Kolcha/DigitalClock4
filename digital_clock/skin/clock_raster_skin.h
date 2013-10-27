#ifndef CLOCK_RASTER_SKIN_H
#define CLOCK_RASTER_SKIN_H

#include <QDir>
#include "raster_skin.h"
#include "clock_base_skin.h"

class ClockRasterSkin : public RasterSkin, public ClockBaseSkin {
public:
  ClockRasterSkin(const QDir& skin_root);

protected:
  void ProcSeparators();
};

#endif // CLOCK_RASTER_SKIN_H
