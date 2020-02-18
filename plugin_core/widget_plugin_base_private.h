/*
    Digital Clock: plugin core library
    Copyright (C) 2016-2020  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef PLUGIN_WIDGET_BASE_PRIVATE_H
#define PLUGIN_WIDGET_BASE_PRIVATE_H

#include <QObject>

#include <QSettings>
#include <QPointer>
#include <QFont>
#include <QVector>

#include "skin_drawer.h"

#include "widget_plugin_settings.h"

class QGridLayout;
class QWidget;

namespace plugin {

class WidgetPluginBase;

/*!
 * @brief WidgetPluginBase implementation details.
 *
 * This class contains 'private' part of WidgetPluginBase class. WidgetPluginBase
 * contains only pointer to this. This class was created to keep binary compatibility
 * when implementation was changed.
 *
 * @note Current implementation assumes that all windows have the same settings and DPI.
 */
class WidgetPluginBasePrivate : public QObject
{
  Q_OBJECT

public:
  /*!
   * Constructor.
   * @param obj - WidgetPluginBase object
   * @param parent - parent object for QObject
   */
  explicit WidgetPluginBasePrivate(WidgetPluginBase* obj, QObject* parent = nullptr);

  /*!
   * Init common plugins settings with default values.
   * @param defaults - map to fill with settings
   * @see WidgetPluginOption, InitDefaults()
   */
  void InitBaseSettingsDefaults(QSettings::SettingsMap* defaults);
  /*!
   * Calculates available width or height, depending on OPT_WIDGET_LOCATION value.
   * @return available width or height
   * @see WidgetLocation, WidgetPluginBase::avail_width_
   */
  int CalculateAvailableSpace() const;

  ::skin_draw::ISkin::SkinPtr currentSkin() const { return skin_; }
  int spacing() const { return spacing_; }

public slots:
  /*!
   * @brief Common option change handler.
   *
   * This can be called only if configurator widget is displayed.
   * BaseSettingsWidget::OptionChanged() is connected to it.
   * @param opt - changed option
   * @param value - new value
   * @see BaseSettingsWidget, WidgetPluginOption
   */
  void onBaseOptionChanged(const WidgetPluginOption opt, const QVariant& value);
  /*!
   * @brief Plugins settings changes listener
   *
   * This can be called as result of interaction with settings dialog or settings import.
   * This is called by onBaseOptionChanged().
   * @param key - changed key
   * @param value - new value
   * @see PluginSettings
   */
  void SettingsChangeListener(const QString& key, const QVariant& value);
  /*!
   * Registers given @a widget internally and performs all required init actions.
   * @see IWidgetPluginInit::Init()
   */
  void AddClockWidget(QWidget* widget);
  /*!
   * Creates all plugin widgets.
   * @see WidgetPluginBase::InitWidget()
   */
  void CreateWidgets();
  /*!
   * Destroys all created plugin widgets.
   * @see CreateWidgets()
   */
  void DestroyWidgets();

  void ApplySkin(::skin_draw::ISkin::SkinPtr skin);
  void SetColor(const QColor& color);
  void SetTexture(const QString& texture);
  void SetCustomizationType(const ::skin_draw::SkinDrawer::CustomizationType ct);
  void SetTexturePerElement(bool enable);
  void SetTextureDrawMode(const ::skin_draw::SkinDrawer::DrawMode dm);
  void SetSpacing(const int spacing);
  void SetZoom(const qreal zoom);

  void DrawText(const QString& text);

private:
  /// clock main window layout
  QVector<QGridLayout*> main_layouts_;
  /// clock main window widget
  QVector<QWidget*> main_wnds_;
  /// all drawers
  QVector< ::skin_draw::SkinDrawer*> drawers_;

  // current drawer settings
  ::skin_draw::ISkin::SkinPtr skin_;
  QColor color_{Qt::magenta};
  QString texture_;
  ::skin_draw::SkinDrawer::CustomizationType customization_{::skin_draw::SkinDrawer::CT_COLOR};
  bool texture_per_element_{true};
  ::skin_draw::SkinDrawer::DrawMode draw_mode_{::skin_draw::SkinDrawer::DM_TILE};
  int spacing_{2};
  qreal zoom_{1.0};

public:
  /// plugin's widget
  QVector<QWidget*> plg_widgets_;
  /// current plugin font (custom font)
  QFont font_;
  /// current clock font
  QFont clock_font_;
  /// current clock skin
  ::skin_draw::ISkin::SkinPtr clock_skin_;
  /// current clock skin customization
  ::skin_draw::SkinDrawer::CustomizationType clock_customization_;
  /// current clock color
  QColor clock_color_;
  /// last rendered text
  QString last_text_;

private:
  ::skin_draw::ISkin::SkinPtr CreateTextSkin(const QFont& fnt);

  WidgetPluginBase* obj_;
};

} // namespace plugin

#endif // PLUGIN_WIDGET_BASE_PRIVATE_H
