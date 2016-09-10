#ifndef PLUGIN_WIDGET_PLUGIN_BASE_H
#define PLUGIN_WIDGET_PLUGIN_BASE_H

#include <QSettings>

#include "iclock_plugin.h"
#include "iplugin_init.h"

#include "plugin_core_global.h"
#include "widget_plugin_settings.h"

class QGridLayout;

namespace plugin {

class WidgetPluginBasePrivate;

class PLUGIN_CORE_SHARED_EXPORT WidgetPluginBase : public IClockPlugin,
    public ISettingsPluginInit,
    public IWidgetPluginInit
{
  Q_OBJECT
  Q_INTERFACES(IClockPlugin ISettingsPluginInit IWidgetPluginInit)

public:
  WidgetPluginBase();
  virtual void Init(const QMap<Option, QVariant>& current_settings);
  virtual void Init(QWidget* main_wnd);

public slots:
  virtual void Start();
  virtual void Stop();
  virtual void SettingsListener(Option option, const QVariant& new_value);
  virtual void TimeUpdateListener();

protected:
  virtual void InitSettingsDefaults(QSettings::SettingsMap* defaults);

  virtual QWidget* InitWidget(QGridLayout* layout) = 0;
  virtual void DisplayImage(const QImage& image) = 0;
  virtual QString GetWidgetText() = 0;

  QWidget* InitConfigWidget(QWidget* parent);

  QSize GetImageSize(const QString& text, qreal zoom) const;

  QString plg_name_;
  int avail_width_;

private:
  virtual qreal CalculateZoom(const QString& text) const;

  // use pointer to private data to keep binary compatibility
  WidgetPluginBasePrivate* private_;
  friend class WidgetPluginBasePrivate;
};

} // namespace plugin

#endif // PLUGIN_WIDGET_PLUGIN_BASE_H
