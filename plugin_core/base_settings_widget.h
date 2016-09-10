#ifndef PLUGIN_BASE_SETTINGS_WIDGET_H
#define PLUGIN_BASE_SETTINGS_WIDGET_H

#include <QWidget>

#include "widget_plugin_settings.h"

class QTranslator;

namespace plugin {

namespace Ui {
class BaseSettingsWidget;
}

/*!
 * @brief Common settings configurator widget.
 *
 * This widget all required controls to change any option from common settings.
 * Special signal will emitted on option change. It already handler by common
 * stuff implementation, normally this is not need to handle it.
 * @see WidgetPluginOption, WidgetPluginBase::InitConfigWidget()
 */
class BaseSettingsWidget : public QWidget
{
  Q_OBJECT

public:
  /*!
   * Constructor.
   * @param parent - parent widget
   */
  explicit BaseSettingsWidget(QWidget* parent = 0);
  /*!
   * Destructor.
   */
  ~BaseSettingsWidget();

  /*!
   * @brief Init controls state.
   *
   * Set controls to state corresponding to current settings.
   * @param cur_values - map with current settings
   */
  void InitWidgets(const QMap<WidgetPluginOption, QVariant>& cur_values);

signals:
  /*!
   * This signal will emitted when any control changes own state.
   * @param opt - changed option
   * @param value - new value
   */
  void OptionChanged(const WidgetPluginOption opt, const QVariant& value);

private slots:
  void on_clock_font_button_toggled(bool checked);
  void on_choose_font_button_clicked();

  void on_not_zoom_clicked();
  void on_font_autosize_clicked();
  void on_clock_zoom_clicked();

  void on_use_custom_color_clicked(bool checked);
  void on_custom_color_button_clicked();

private:
  Ui::BaseSettingsWidget* ui;
  QFont last_font_;
  QColor last_color_;
  QTranslator* gui_translator_;
};

} // namespace plugin

#endif // PLUGIN_BASE_SETTINGS_WIDGET_H
