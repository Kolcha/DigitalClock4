#ifndef PLUGIN_WIDGET_BASE_PRIVATE_H
#define PLUGIN_WIDGET_BASE_PRIVATE_H

#include <QObject>

#include <QSettings>
#include <QPointer>
#include <QFont>

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

public:
  /// clock main window layout
  QGridLayout* main_layout_;
  /// clock main window widget
  QWidget* main_wnd_;
  /// plugin's widget
  QPointer<QWidget> plg_widget_;
  /// current plugin font (custom font)
  QFont font_;
  /// current clock font
  QFont clock_font_;
  /// current clock zoom
  qreal clock_zoom_;
  /// current clock skin customization
  ::skin_draw::SkinDrawer::CustomizationType clock_customization_;
  /// current clock color
  QColor clock_color_;
  /// last rendered text
  QString last_text_;
  /// text render
  ::skin_draw::SkinDrawer* drawer_;

private:
  WidgetPluginBase* obj_;
};

} // namespace plugin

#endif // PLUGIN_WIDGET_BASE_PRIVATE_H
