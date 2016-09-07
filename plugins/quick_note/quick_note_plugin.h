#ifndef QUICK_NOTE_QUICK_NOTE_PLUGIN_H
#define QUICK_NOTE_QUICK_NOTE_PLUGIN_H

#include "widget_plugin_base.h"

class QGridLayout;

namespace quick_note {

class MessageWidget;

class QuickNotePlugin : public ::plugin::WidgetPluginBase
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "quick_note.json")

public:
  QuickNotePlugin();

  void Init(const QMap<Option, QVariant>& current_settings);

public slots:
  void Configure();

  void SettingsListener(Option option, const QVariant& new_value);

protected:
  void InitSettingsDefaults(QSettings::SettingsMap* defaults);
  QWidget* InitWidget(QGridLayout* layout);
  void DisplayImage(const QImage& image);
  QString GetWidgetText();

private slots:
  void SettingsUpdateListener(const QString& key, const QVariant& value);

private:
  qreal CalculateZoom(const QString& text) const;

  MessageWidget* msg_widget_;
  QColor msg_color_;
};

} // namespace quick_note

#endif // QUICK_NOTE_QUICK_NOTE_PLUGIN_H
