#ifndef QUICK_NOTE_H
#define QUICK_NOTE_H

#include <QFont>
#include <QPointer>
#include "iclock_plugin.h"
#include "iplugin_init.h"

class QGridLayout;
class PluginSettings;
namespace skin_draw {
class SkinDrawer;
}

namespace quick_note {

class MessageWidget;

class QuickNote : public IClockPlugin,
                  public ISettingsPluginInit,
                  public IWidgetPluginInit {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "quick_note.json")
  Q_INTERFACES(IClockPlugin ISettingsPluginInit IWidgetPluginInit)

public:
  QuickNote();
  void Init(const QMap<Option, QVariant>& current_settings);
  void Init(QWidget* main_wnd);

  void ExportSettings(QSettings::SettingsMap* settings);
  void ImportSettings(const QSettings::SettingsMap& settings);

public slots:
  void Start();
  void Stop();
  void Configure();
  void SettingsListener(Option option, const QVariant& new_value);

private slots:
  void ApplyString(const QString& str);

private:
  PluginSettings* settings_;
  QGridLayout* main_layout_;
  QWidget* main_wnd_;
  QPointer<MessageWidget> msg_label_;
  QFont font_;
  int avail_width_;
  qreal last_zoom_;
  ::skin_draw::SkinDrawer* drawer_;
  QColor msg_color_;
};

} // namespace quick_note

#endif // QUICK_NOTE_H
