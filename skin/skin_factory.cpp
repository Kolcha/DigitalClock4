#include "raster_skin.h"
#include "vector_skin.h"

IClockSkin* CreateSkin(const QDir& skin_root) {
  QStringList images = skin_root.entryList(QStringList("*.svg"), QDir::Files);
  QStringList skinini = skin_root.entryList(QStringList("skin.ini"), QDir::Files);
  IClockSkin* skin = 0;
  if (!images.empty() && !skinini.empty()) skin = new VectorSkin(skin_root);
  images = skin_root.entryList(QStringList("*.png"), QDir::Files);
  if (!images.empty() && !skinini.empty()) skin = new RasterSkin(skin_root);
  return skin;
}
