/*
    Digital Clock: skin draw engine
    Copyright (C) 2013-2017  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SKIN_DRAW_SKIN_DRAWER_H
#define SKIN_DRAW_SKIN_DRAWER_H

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
class SKIN_DRAWSHARED_EXPORT SkinDrawer : public QObject
{
  Q_OBJECT

  Q_PROPERTY(ISkin::SkinPtr skin READ currentSkin WRITE ApplySkin NOTIFY skinChanged)
  Q_PROPERTY(QString text READ text WRITE SetString NOTIFY textChanged)
  Q_PROPERTY(qreal zoom READ zoom WRITE SetZoom NOTIFY zoomChanged)
  Q_PROPERTY(int spacing READ spacing WRITE SetSpace NOTIFY spacingChanged)
  Q_PROPERTY(CustomizationType customization READ customization WRITE SetCustomizationType NOTIFY customizationChanged)
  Q_PROPERTY(QColor color READ color WRITE SetColor NOTIFY colorChanged)
  Q_PROPERTY(QString textureFile READ textureFile WRITE SetTexture NOTIFY textureFileChanged)
  Q_PROPERTY(QPixmap texture READ texture NOTIFY textureChanged)
  Q_PROPERTY(bool texturePerElement READ texturePerElement WRITE SetTexturePerElement NOTIFY texturePerElementChanged)
  Q_PROPERTY(DrawMode textureMode READ textureMode WRITE SetTextureDrawMode NOTIFY textureModeChanged)
  Q_PROPERTY(bool previewMode READ previewMode WRITE SetPreviewMode NOTIFY previewModeChanged)

public:
  /*!
   * Constructor.
   * @param parent - parent object
   */
  explicit SkinDrawer(QObject* parent = nullptr);

  /*! texture draw modes */
  enum DrawMode {
    DM_STRETCH,     /*!< stretch texture to fill needed area */
    DM_TILE         /*!< draw tiled texure image */
  };
  Q_ENUM(DrawMode)
  /*! customization types */
  enum CustomizationType {
    CT_NONE,        /*!< do not use any customization, draw skin as is */
    CT_COLOR,       /*!< draw skin with custom color */
    CT_TEXTURE      /*!< draw skin with custom texture */
  };
  Q_ENUM(CustomizationType)

  /*!
   * @property SkinDrawer::skin
   * current skin
   */
  ISkin::SkinPtr currentSkin() const { return skin_; }
  /*!
   * @property SkinDrawer::text
   * current drawn text
   */
  QString text() const { return str_; }
  /*!
   * @property SkinDrawer::zoom
   * current zoom
   * @note default value is 1.0
   */
  qreal zoom() const { return zoom_; }
  /*!
   * @property SkinDrawer::spacing
   * space between digits in both directions
   * @note default value is 4
   */
  int spacing() const { return space_; }
  /*!
   * @property SkinDrawer::customization
   * current skin customization type
   * @note default value is CustomizationType::CT_COLOR
   * @see SkinDrawer::CustomizationType
   */
  CustomizationType customization() const { return cust_type_; }
  /*!
   * @property SkinDrawer::color
   * current color for skin customization
   * @note default value is Qt::magenta
   * @see SkinDrawer::customization, SkinDrawer::texture
   */
  QColor color() const { return color_; }
  /*!
   * @property SkinDrawer::textureFile
   * current texture file for skin customization
   * @see SkinDrawer::customization, SkinDrawer::texture
   */
  QString textureFile() const { return txd_file_; }
  /*!
   * @property SkinDrawer::texture
   * @brief current texture used for skin customization
   *
   * If color is used for customization, it is a 8x8 pixmap filled with SkinDrawer::color,
   * otherwise it contains image data loaded from SkinDrawer::textureFile.
   * This property may have invalid value in last case.
   *
   * @note This is read-only property, there is no way to set it explicitly.
   * @see SkinDrawer::customization, SkinDrawer::color, SkinDrawer::textureFile
   */
  QPixmap texture() const { return texture_; }
  /*!
   * @property SkinDrawer::texturePerElement
   * @brief texturing parameter
   *
   * If set to @e true texture will be applied for each text element separately
   * instead of whole text at once.
   *
   * Disabled by default, i.e. texture will be applied for whole text at once.
   * Enabling it may decrease performance slightly.
   *
   * @see SkinDrawer::textureMode
   */
  bool texturePerElement() const { return txd_per_elem_; }
  /*!
   * @property SkinDrawer::textureMode
   * @brief texturing parameter
   *
   * Influences how texture will be modified to fill required space.
   * Default value is set to DrawMode::DM_TILE.
   *
   * @see SkinDrawer::DrawMode, SkinDrawer::texturePerElement
   */
  DrawMode textureMode() const { return txd_draw_mode_; }
  /*!
   * @property SkinDrawer::previewMode
   * @brief indicates is SkinDrawer in preview mode
   *
   * When preview mode is enabled, almost all rendering optimizations are disabled,
   * i.e. each paint request will be actually performed each time.
   *
   * Enabling this mode may be useful when paramenters are changed to often. For example,
   * when settings dialog is open and user tinkering with its controls.
   *
   * It is disabled by default.
   */
  bool previewMode() const { return preview_mode_; }

signals:
  /*!
   * @fn void SkinDrawer::skinChanged(ISkin::SkinPtr skin)
   * This signal is emitted when the current skin has changed, with the new @a skin as an argument.
   */
  void skinChanged(ISkin::SkinPtr skin);
  /*!
   * @fn void SkinDrawer::textChanged(const QString& text)
   * This signal is emitted when the current drawing text has changed, with the new @a text as an argument.
   */
  void textChanged(const QString& text);
  /*!
   * @fn void SkinDrawer::zoomChanged(qreal zoom)
   * This signal is emitted when the current zoom has changed, with the new @a zoom as an argument.
   */
  void zoomChanged(qreal zoom);
  /*!
   * @fn void SkinDrawer::spacingChanged(int value)
   * This signal is emitted when the spacing has changed, with the new @a value as an argument.
   */
  void spacingChanged(int value);
  /*!
   * @fn void SkinDrawer::customizationChanged(CustomizationType type)
   * This signal is emitted when the customization type has changed, with the new @a type as an argument.
   */
  void customizationChanged(CustomizationType type);
  /*!
   * @fn void SkinDrawer::colorChanged(const QColor& color)
   * This signal is emitted when the current color has changed, with the new @a color as an argument.
   */
  void colorChanged(const QColor& color);
  /*!
   * @fn void SkinDrawer::textureFileChanged(const QString& file)
   * This signal is emitted when the current texture file has changed, with the new @a file as an argument.
   */
  void textureFileChanged(const QString& file);
  /*!
   * @fn void SkinDrawer::textureChanged(const QPixmap& texture)
   * This signal is emitted when the current texture has changed, with the new @a texture as an argument.
   */
  void textureChanged(const QPixmap& texture);
  /*!
   * @fn void SkinDrawer::texturePerElementChanged(bool state)
   * This signal is emitted when texturePerElement property has changed, with the new @a state as an argument.
   */
  void texturePerElementChanged(bool state);
  /*!
   * @fn void SkinDrawer::textureModeChanged(DrawMode mode)
   * This signal is emitted when textureMode property has changed, with the new @a mode as an argument.
   */
  void textureModeChanged(DrawMode mode);
  /*!
   * @fn void SkinDrawer::previewModeChanged(bool state)
   * This signal is emitted when the preview mode has toggled, with the new @a state as an argument.
   */
  void previewModeChanged(bool state);
  /*!
    @fn void SkinDrawer::DrawingFinished(const QImage& result)
   * This signal is emitted when text drawing has finished, with the @a result image as argument.
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
   */
  void SetColor(const QColor& new_color);
  /*!
   * Set texture to use.
   * @param filename - path to texture image file
   */
  void SetTexture(const QString& filename);
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
   * @param mode - new draw mode
   * @see DrawMode description for details and possible values.
   */
  void SetTextureDrawMode(DrawMode mode);
  /*!
   * @brief Set skin customization type.
   *
   * SkinDrawer can "customize" skin appling texture or color for it. Its behaviour depends on this value.
   * @param type - new customization type
   * @see CustomizationType for possible values and its description.
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
   * Generate texture pixmap using given @a color.
   * @return @e true on success, otherwise return @e false
   */
  bool UpdateTexture(const QColor& color);
  /*!
   * Load texture pixmap from given @a file.
   * @return @e true on success, otherwise return @e false
   */
  bool UpdateTexture(const QString& file);
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

#endif // SKIN_DRAW_SKIN_DRAWER_H
