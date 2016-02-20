#ifndef SKIN_DRAWER_H
#define SKIN_DRAWER_H

#include <QObject>

#include <QImage>
#include <QColor>
#include <QString>

#include "iskin.h"
#include "skin_draw_global.h"

namespace skin_draw {

/*!
 * @brief Skin draw engine.
 *
 * SkinDrawer can draw any given text using some skin. Skin can be customized.
 * It draws all symbols one by one, applying needed changes for images from skin.
 * All changed images is cached by default.
 */
class SKIN_DRAWSHARED_EXPORT SkinDrawer : public QObject {
  Q_OBJECT
public:
  /*!
   * Constructor.
   * @param parent - parent object
   */
  explicit SkinDrawer(QObject* parent = 0);

  /*! texture draw modes */
  enum DrawMode {
    DM_STRETCH,     /*!< stretch texture to fill needed area */
    DM_TILE         /*!< draw tiled texure image */
  };
  /*! customization types */
  enum CustomizationType {
    CT_NONE,        /*!< do not use any customization, draw skin as is */
    CT_COLOR,       /*!< draw skin with custom color */
    CT_TEXTURE      /*!< draw skin with custom texture */
  };

signals:
  /*!
   * This signal is emitted when text drawing is finished.
   * @param result - result image with drawed text
   */
  void DrawingFinished(const QImage& result);

public slots:
  /*!
   * Apply skin for text.
   * @param skin - skin to apply
   */
  void ApplySkin(ISkin::SkinPtr skin);
  /*!
   * Set string to draw.
   * @param str - string to draw
   */
  void SetString(const QString& str);
  /*!
   * Set skin zoom.
   * @param new_zoom - new zoom
   */
  void SetZoom(qreal new_zoom);
  /*!
   * Set color to use.
   * @param new_color - new color to use
   * @return true if set is passed, otherwise returns false
   */
  bool SetColor(const QColor& new_color);
  /*!
   * Set texture to use.
   * @param filename - path to texture image file
   * @return true if set is passed, otherwise returns false
   */
  bool SetTexture(const QString& filename);
  /*!
   * @brief Set texture per element.
   *
   * When it enabled, texture is applied for each element instead of whole text.
   * @param set - true to enable, false to disable
   */
  void SetTexturePerElement(bool set);
  /*!
   * @brief Set texture draw mode.
   *
   * SkinDrawer can draw texture by different ways. This paramenter chooses one of them.
   * @see DrawMode description for details and possible values.
   * @param mode - new draw mode
   */
  void SetTextureDrawMode(DrawMode mode);
  /*!
   * @brief Set skin customization type.
   *
   * SkinDrawer can "customize" skin appling texture or color for it. Its behaviour depends on
   * this value. @see CustomizationType for possible values and its description.
   * @param type - new customization type
   */
  void SetCustomizationType(CustomizationType type);
  /*!
   * @brief Set space between neighbor symbols.
   *
   * Default value is 4 pixels.
   * @param new_space - new value in pixels
   */
  void SetSpace(int new_space);
  /*!
   * @brief Enable/disable preview mode.
   *
   * Preview mode is enabled only when user changes clock settings. Disabled by default.
   * @param set - true - enable preview mode, false - disable
   * @note When preview mode is enabled, all transformed images are not cached.
   */
  void SetPreviewMode(bool set);

private:
  /*!
   * Redraw whole image with text using new options.
   */
  void Redraw();
  /*!
   * Draw texture.
   * @param painter - QPainter object
   * @param rect - area to draw texture
   */
  void DrawTexture(QPainter& painter, const QRect& rect);

  /// device pixel ratio, see Qt documentation for details
  qreal device_pixel_ratio_;

  ISkin::SkinPtr skin_;
  QString str_;
  qreal zoom_;
  QPixmap texture_;
  bool txd_per_elem_;
  DrawMode txd_draw_mode_;
  bool preview_mode_;
  QString txd_file_;
  QColor color_;
  CustomizationType cust_type_;
  int space_;
};

} // namespace skin_draw

#endif // SKIN_DRAWER_H
