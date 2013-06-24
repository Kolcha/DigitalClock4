#include <QDir>
#include "raster_skin.h"
#include "vector_skin.h"

IClockSkin* CreateSkin(const QString& skin_root) {
  QDir root(skin_root);
  QStringList images = root.entryList(QStringList("*.svg"), QDir::Files);
  if (images.empty()) {
    return new RasterSkin(skin_root);
  } else {
    return new VectorSkin(skin_root);
  }
}
