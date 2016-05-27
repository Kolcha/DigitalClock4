#ifndef IP_ADDRESS_IP_ADDRESS_H
#define IP_ADDRESS_IP_ADDRESS_H

#include "iclock_plugin.h"
#include "iplugin_init.h"

#include <QFont>
#include <QPointer>

class QGridLayout;
class QLabel;
namespace skin_draw {
class SkinDrawer;
}

namespace ip_address {

class IPAddress : public IClockPlugin,
                  public ISettingsPluginInit,
                  public IWidgetPluginInit {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "ip_address.json")
  Q_INTERFACES(IClockPlugin ISettingsPluginInit IWidgetPluginInit)

public:
  IPAddress();
  void Init(const QMap<Option, QVariant>& current_settings);
  void Init(QWidget* main_wnd);

public slots:
  void Start();
  void Stop();
  void SettingsListener(Option option, const QVariant& new_value);

private slots:
  void ApplyString(const QString& str);

private:
  QString ip_list_;
  QGridLayout* main_layout_;
  QWidget* main_wnd_;
  QPointer<QLabel> msg_label_;
  QFont font_;
  int avail_width_;
  qreal last_zoom_;
  ::skin_draw::SkinDrawer* drawer_;
  QColor msg_color_;
};

} // namespace ip_address

#endif // IP_ADDRESS_IP_ADDRESS_H
