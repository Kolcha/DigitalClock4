#ifndef SKIN_DRAWER_H
#define SKIN_DRAWER_H

#include "skin_draw_global.h"

#include <QPixmap>
#include "iclock_skin.h"
// TODO: add comments
// TODO: variable spacing
// TODO: add ApplySkin(Skin* skin) instead of LoadSkin()
class SKIN_DRAWSHARED_EXPORT SkinDrawer : public QObject {
  Q_OBJECT
public:
  explicit SkinDrawer(QObject* parent = 0);
  ~SkinDrawer();

  enum DrawMode {DM_STRETCH, DM_TILE};
  enum CustomizationType {CT_NONE, CT_COLOR, CT_TEXTURE};

signals:
  void DrawingFinished(const QImage& result);
  void LoadedSkinInfo(const TSkinInfo& info);

public slots:
  void LoadSkin(const QDir& skin_root);
  void LoadSkin(const QFont& font);
  void SetString(const QString& str);
  void SetZoom(qreal new_zoom);
  bool SetColor(const QColor& new_color);
  bool SetTexture(const QString& filename);
  void SetTexturePerElement(bool set);
  void SetTextureDrawMode(DrawMode mode);
  void SetCustomizationType(CustomizationType type);
  void SetPreviewMode(bool set);

private:
  void Redraw();
  void DrawTexture(QPainter& painter, const QRect& rect);
  IClockSkin* skin_;
  QString str_;
  qreal zoom_;
  QPixmap texture_;
  bool txd_per_elem_;
  DrawMode txd_draw_mode_;
  bool preview_mode_;
  QString txd_file_;
  QColor color_;
  CustomizationType cust_type_;
};

#endif // SKIN_DRAWER_H
