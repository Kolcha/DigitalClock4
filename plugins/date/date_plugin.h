#ifndef DATE_DATEPLUGIN_H
#define DATE_DATEPLUGIN_H

#include "widget_plugin_base.h"

class QGridLayout;
class QLabel;

namespace date {

class DatePlugin : public ::plugin::WidgetPluginBase
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "date.json")

public:
    DatePlugin();

public slots:
    void Configure();

protected:
    void InitSettingsDefaults(QSettings::SettingsMap* defaults);
    QWidget* InitWidget(QGridLayout* layout);
    void DisplayImage(const QImage& image);
    QString GetWidgetText();

private:
    QLabel* msg_label_;
};

} // namespace date

#endif // DATE_DATEPLUGIN_H
