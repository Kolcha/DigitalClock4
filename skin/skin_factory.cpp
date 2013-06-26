#include <QDir>
#include "raster_skin.h"
#include "vector_skin.h"

IClockSkin* CreateSkin(const QString& skin_root) {
  QDir root(skin_root);
  QStringList images = root.entryList(QStringList("*.svg"), QDir::Files);

  IClockSkin* skin = 0;
  if (!images.empty()) skin = new VectorSkin(skin_root);
  images = root.entryList(QStringList("*.png"), QDir::Files);
  if (!images.empty()) skin = new RasterSkin(skin_root);
  return skin;
}
