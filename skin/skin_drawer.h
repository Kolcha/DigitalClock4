#ifndef SKIN_DRAWER_H
#define SKIN_DRAWER_H

#include "iclock_skin.h"

class SkinDrawer : public QObject {
  Q_OBJECT
public:
  explicit SkinDrawer(QObject *parent = 0);
  ~SkinDrawer();

  QPixmap* DrawString(const QString& str);

public slots:
  void LoadSkin(const QString& skin_root);
  void SetZoom(qreal new_zoom);
  void SetColor(QColor new_color);
  void SetTexture(const QString& filename);
  void SetTexturePerElement(bool set);

private:
  IClockSkin* skin_;
};

#endif // SKIN_DRAWER_H
