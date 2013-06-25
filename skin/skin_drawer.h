#ifndef SKIN_DRAWER_H
#define SKIN_DRAWER_H

#include "iclock_skin.h"

class SkinDrawer : public QObject {
  Q_OBJECT
public:
  explicit SkinDrawer(QObject *parent = 0);
  ~SkinDrawer();

signals:
  void DrawingFinished(QPixmap* result);

public slots:
  void LoadSkin(const QString& skin_root);
  void SetString(const QString& str);
  void SetZoom(qreal new_zoom);
  void SetColor(const QColor& new_color);
  void SetTexture(const QString& filename);
  void SetTexturePerElement(bool set);
  void SetPreviewMode(bool set);

private:
  void Redraw();
  QPixmap* result_;
  IClockSkin* skin_;
  QString str_;
  qreal zoom_;
  QPixmap texture_;
  bool txd_per_elem_;
  bool preview_mode_;
};

#endif // SKIN_DRAWER_H
