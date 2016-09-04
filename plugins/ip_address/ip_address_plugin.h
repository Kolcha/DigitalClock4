#ifndef IP_ADDRESS_IP_ADDRESS_PLUGIN_H
#define IP_ADDRESS_IP_ADDRESS_PLUGIN_H

#include "widget_plugin_base.h"

class QGridLayout;
class QLabel;

namespace ip_address {

class IpAddressPlugin : public ::plugin::WidgetPluginBase
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "ip_address.json")

public:
    IpAddressPlugin();

public slots:
    void Configure();

protected:
    QWidget* InitWidget(QGridLayout* layout);
    void DisplayImage(const QImage& image);
    QString GetWidgetText();

private:
    QLabel* msg_label_;
};

} // namespace ip_address

#endif // IP_ADDRESS_IP_ADDRESS_PLUGIN_H
