#ifndef PLUGIN_BASE_SETTINGS_WIDGET_H
#define PLUGIN_BASE_SETTINGS_WIDGET_H

#include <QWidget>

#include "widget_plugin_settings.h"

class QTranslator;

namespace plugin {

namespace Ui {
class BaseSettingsWidget;
}

class BaseSettingsWidget : public QWidget
{
  Q_OBJECT

public:
  explicit BaseSettingsWidget(QWidget* parent = 0);
  ~BaseSettingsWidget();

  void InitWidgets(const QMap<WidgetPluginOption, QVariant>& cur_values);

signals:
  void OptionChanged(const WidgetPluginOption, const QVariant&);

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
