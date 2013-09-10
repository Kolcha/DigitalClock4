#include "raster_skin.h"
#include "vector_skin.h"
#include "text_skin.h"

IClockSkin* CreateSkin(const QDir& skin_root) {
  QStringList images = skin_root.entryList(QStringList("*.svg"), QDir::Files);
  bool skinini = skin_root.exists("skin.ini");
  IClockSkin* skin = 0;
  if (!images.empty() && skinini) skin = new VectorSkin(skin_root);
  images = skin_root.entryList(QStringList("*.png"), QDir::Files);
  if (!images.empty() && skinini) skin = new RasterSkin(skin_root);
  return skin;
}

IClockSkin* CreateSkin(const QFont& font) {
  return new TextSkin(font);
}
