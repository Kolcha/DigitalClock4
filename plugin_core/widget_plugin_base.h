#ifndef PLUGIN_WIDGET_PLUGIN_BASE_H
#define PLUGIN_WIDGET_PLUGIN_BASE_H

#include <QFont>
#include <QSettings>

#include "iclock_plugin.h"
#include "iplugin_init.h"

#include "plugin_core_global.h"
#include "widget_plugin_settings.h"

class QGridLayout;
namespace skin_draw {
class SkinDrawer;
}

namespace plugin {

class PLUGIN_CORE_SHARED_EXPORT WidgetPluginBase : public IClockPlugin,
        public ISettingsPluginInit,
        public IWidgetPluginInit {
    Q_OBJECT
    Q_INTERFACES(IClockPlugin ISettingsPluginInit IWidgetPluginInit)

public:
    WidgetPluginBase();
    void Init(const QMap<Option, QVariant>& current_settings);
    void Init(QWidget* main_wnd);

public slots:
    void Start();
    void Stop();
    void SettingsListener(Option option, const QVariant& new_value);
    void TimeUpdateListener();

protected:
    virtual void InitSettingsDefaults(QSettings::SettingsMap* defaults);

    virtual QWidget* InitWidget(QGridLayout* layout) = 0;
    virtual void DisplayImage(const QImage& image) = 0;
    virtual QString GetWidgetText() = 0;

    QWidget* InitConfigWidget(QWidget* parent);

    QSize GetImageSize(const QString& text, qreal zoom) const;

    QString plg_name_;
    int avail_width_;

private slots:
    void onBaseOptionChanged(const WidgetPluginOption opt, const QVariant& value);
    void SettingsChangeListener(const QString& key, const QVariant& value);

private:
    void InitBaseSettingsDefaults(QSettings::SettingsMap* defaults);
    virtual qreal CalculateZoom(const QString& text) const;

    QGridLayout* main_layout_;
    QWidget* main_wnd_;
    QPointer<QWidget> plg_widget_;
    QFont font_;
    QFont clock_font_;
    qreal clock_zoom_;
    QString last_text_;
    ::skin_draw::SkinDrawer* drawer_;
};

} // namespace plugin

#endif // PLUGIN_WIDGET_PLUGIN_BASE_H
