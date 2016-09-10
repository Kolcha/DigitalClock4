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

class WidgetPluginBasePrivate : public QObject
{
  Q_OBJECT

public:
  explicit WidgetPluginBasePrivate(WidgetPluginBase* obj, QObject* parent = nullptr);

  void InitBaseSettingsDefaults(QSettings::SettingsMap* defaults);

public slots:
  void onBaseOptionChanged(const WidgetPluginOption opt, const QVariant& value);
  void SettingsChangeListener(const QString& key, const QVariant& value);

public:
  QGridLayout* main_layout_;
  QWidget* main_wnd_;
  QPointer<QWidget> plg_widget_;
  QFont font_;
  QFont clock_font_;
  qreal clock_zoom_;
  ::skin_draw::SkinDrawer::CustomizationType clock_customization_;
  QColor clock_color_;
  QString last_text_;
  ::skin_draw::SkinDrawer* drawer_;

private:
  WidgetPluginBase* obj_;
};

} // namespace plugin

#endif // PLUGIN_WIDGET_BASE_PRIVATE_H
