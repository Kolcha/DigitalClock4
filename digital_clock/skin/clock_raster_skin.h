#ifndef CLOCK_RASTER_SKIN_H
#define CLOCK_RASTER_SKIN_H

#include <QDir>
#include "raster_skin.h"

class ClockRasterSkin : public RasterSkin {
public:
  ClockRasterSkin(const QDir& skin_root);
};

#endif // CLOCK_RASTER_SKIN_H
