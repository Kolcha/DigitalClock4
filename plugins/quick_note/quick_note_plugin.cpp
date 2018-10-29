/*
    Digital Clock: quick note plugin
    Copyright (C) 2016-2018  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "quick_note_plugin.h"

#include <QVBoxLayout>

#include "plugin_settings.h"

#include "quick_note_settings.h"
#include "gui/message_widget.h"
#include "gui/settings_dialog.h"

namespace quick_note {

QuickNotePlugin::QuickNotePlugin()
{
  InitTranslator(QLatin1String(":/quick_note/lang/quick_note_"));
  info_.display_name = tr("Quick note");
  info_.description = tr("Allows to display any short message under clock.");
  InitIcon(":/quick_note/icon.svg.p");
}

void QuickNotePlugin::Stop()
{
  ::plugin::WidgetPluginBase::Stop();
  msg_widgets_.clear();
}

void QuickNotePlugin::Configure()
{
  SettingsDialog* dialog = new SettingsDialog();
  connect(dialog, &SettingsDialog::destroyed, this, &QuickNotePlugin::configured);
  // load current settings to dialog
  QSettings::SettingsMap curr_settings;
  InitDefaults(&curr_settings);
  for (auto iter = curr_settings.begin(); iter != curr_settings.end(); ++iter) {
    *iter = settings_->GetOption(iter.key());
  }
  dialog->Init(curr_settings);
  // add widget with common settings configuration controls
  QVBoxLayout* dlg_layout = qobject_cast<QVBoxLayout*>(dialog->layout());
  Q_ASSERT(dlg_layout);
  dlg_layout->insertWidget(dlg_layout->count() - 1, InitConfigWidget(dialog));
  // connect main signals/slots
  connect(dialog, SIGNAL(OptionChanged(QString,QVariant)),
          settings_, SLOT(SetOption(QString,QVariant)));
  connect(dialog, SIGNAL(accepted()), settings_, SLOT(Save()));
  connect(dialog, SIGNAL(rejected()), settings_, SLOT(Load()));
  dialog->show();
}

void QuickNotePlugin::InitSettingsDefaults(QSettings::SettingsMap* defaults)
{
  InitDefaults(defaults);
}

QWidget* QuickNotePlugin::InitWidget(QGridLayout* layout)
{
  MessageWidget* msg_widget = new MessageWidget();
  msg_widgets_.append(msg_widget);
  connect(msg_widget, &MessageWidget::textEdited, [this] (const QString& str) {
    settings_->SetOption(OPT_QUICK_NOTE_MSG, str);
    settings_->Save();
  });
  connect(msg_widget, &MessageWidget::textChanged, this, &QuickNotePlugin::TimeUpdateListener);
  connect(settings_, &PluginSettings::OptionChanged, this, &QuickNotePlugin::SettingsUpdateListener);
  Q_UNUSED(layout);
  return msg_widget;
}

void QuickNotePlugin::DisplayImage(QWidget* widget, const QImage& image)
{
  static_cast<MessageWidget*>(widget)->setPixmap(QPixmap::fromImage(image));
}

QString QuickNotePlugin::GetWidgetText()
{
  return settings_->GetOption(OPT_QUICK_NOTE_MSG).toString();
}

void QuickNotePlugin::SettingsUpdateListener(const QString& key, const QVariant& value)
{
  if (key == OPT_QUICK_NOTE_MSG) {
    for (auto& msg_widget : qAsConst(msg_widgets_))
      msg_widget->setText(value.toString());
  }
}

} // namespace quick_note
