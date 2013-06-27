#ifndef SKIN_DRAWER_H
#define SKIN_DRAWER_H

#include <QPixmap>
#include "iclock_skin.h"

class SkinDrawer : public QObject {
  Q_OBJECT
public:
  explicit SkinDrawer(QObject* parent = 0);
  ~SkinDrawer();

  enum DrawMode {DM_STRETCH, DM_TILE};

signals:
  void DrawingFinished(const QPixmap& result);
  void LoadedSkinInfo(const TSkinInfo& info);

public slots:
  void LoadSkin(const QDir& skin_root);
  void SetString(const QString& str);
  void SetZoom(qreal new_zoom);
  void SetColor(const QColor& new_color);
  void SetTexture(const QString& filename);
  void SetTexturePerElement(bool set);
  void SetTextureDrawMode(DrawMode mode);
  void SetUseTexture(bool set);
  void SetPreviewMode(bool set);

private:
  void Redraw();
  void DrawTexture(QPainter& painter, const QRect& rect);
  IClockSkin* skin_;
  QString str_;
  qreal zoom_;
  QPixmap texture_;
  bool txd_per_elem_;
  bool use_txd_;
  DrawMode txd_draw_mode_;
  bool preview_mode_;
  QString txd_file_;
};

#endif // SKIN_DRAWER_H
