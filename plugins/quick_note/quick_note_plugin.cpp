/*
    Digital Clock: quick note plugin
    Copyright (C) 2013-2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#include <QGridLayout>
#include <QVBoxLayout>

#include "plugin_settings.h"

#include "quick_note_settings.h"
#include "gui/message_widget.h"
#include "gui/settings_dialog.h"

namespace quick_note {

QuickNotePlugin::QuickNotePlugin() : msg_widget_(nullptr)
{
  InitTranslator(QLatin1String(":/quick_note/quick_note_"));
  info_.display_name = tr("Quick note");
  info_.description = tr("Allows to display any short message under clock.");
  InitIcon(":/quick_note/icon.svg");
  plg_name_ = QString("quick_note");
}

void QuickNotePlugin::Init(const QMap<Option, QVariant>& current_settings)
{
  for (auto iter = current_settings.begin(); iter != current_settings.end(); ++iter) {
    switch (iter.key()) {
      case OPT_COLOR:
        msg_color_ = iter.value().value<QColor>();
        break;

      default:
        break;
    }
  }
  WidgetPluginBase::Init(current_settings);
}

void QuickNotePlugin::Configure()
{
  SettingsDialog* dialog = new SettingsDialog();
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

void QuickNotePlugin::SettingsListener(Option option, const QVariant& new_value)
{
  switch (option) {
    case OPT_COLOR:
      msg_color_ = new_value.value<QColor>();
      if (msg_widget_) msg_widget_->setIconColor(msg_color_);
      break;
    default:
      break;
  }
  WidgetPluginBase::SettingsListener(option, new_value);
}

void QuickNotePlugin::InitSettingsDefaults(QSettings::SettingsMap* defaults)
{
  InitDefaults(defaults);
}

QWidget* QuickNotePlugin::InitWidget(QGridLayout* layout)
{
  msg_widget_ = new MessageWidget();
  connect(msg_widget_, &MessageWidget::textEdited, [=] (const QString& str) {
    settings_->SetOption(OPT_QUICK_NOTE_MSG, str);
    settings_->Save();
  });
  msg_widget_->setIconColor(msg_color_);
  connect(msg_widget_, &MessageWidget::textChanged, this, &QuickNotePlugin::TimeUpdateListener);
  connect(settings_, &PluginSettings::OptionChanged, this, &QuickNotePlugin::SettingsUpdateListener);
  layout->addWidget(msg_widget_, layout->rowCount(), 0, 1, layout->columnCount());
  return msg_widget_;
}

void QuickNotePlugin::DisplayImage(const QImage& image)
{
  msg_widget_->setPixmap(QPixmap::fromImage(image));
}

QString QuickNotePlugin::GetWidgetText()
{
  return settings_->GetOption(OPT_QUICK_NOTE_MSG).toString();
}

void QuickNotePlugin::SettingsUpdateListener(const QString& key, const QVariant& value)
{
  if (key == OPT_QUICK_NOTE_MSG) {
    msg_widget_->setText(value.toString());
  }
  if (key == OPT_SHOW_EDIT_BTN) {
    msg_widget_->ShowEditButton(value.toBool());
  }

  using ::plugin::OptionKey;
  using ::plugin::OPT_USE_CUSTOM_COLOR;
  using ::plugin::OPT_CUSTOM_COLOR;

  if (key == OptionKey(OPT_USE_CUSTOM_COLOR, plg_name_)) {
    if (value.toBool()) {
      QColor cc = settings_->GetOption(OptionKey(OPT_CUSTOM_COLOR, plg_name_)).value<QColor>();
      msg_widget_->setIconColor(cc);
    } else {
      msg_widget_->setIconColor(msg_color_);
    }
  }
  if (key == OptionKey(OPT_CUSTOM_COLOR, plg_name_)) {
    if (settings_->GetOption(OptionKey(OPT_USE_CUSTOM_COLOR, plg_name_)).toBool()) {
      msg_widget_->setIconColor(value.value<QColor>());
    }
  }
}

qreal QuickNotePlugin::CalculateZoom(const QString& text) const
{
  QSize s0 = GetImageSize(text, 1.0);
  qreal req_img_w = avail_width_ - msg_widget_->layout()->spacing() - 2*msg_widget_->layout()->margin();
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
  qreal dpr = msg_widget_->devicePixelRatioF();
#else
  int dpr = msg_widget_->devicePixelRatio();
#endif
  qreal c_zoom = req_img_w * dpr / s0.width();

  QSize s = GetImageSize(text, c_zoom);
  int ww = (s.width() + 0.75*s.height()) / dpr + 2*msg_widget_->layout()->margin() + msg_widget_->layout()->spacing();
  while ((ww > avail_width_) && (ww - avail_width_) > 4) {
    c_zoom *= (1 - (0.5*(ww - avail_width_)) / avail_width_);
    s = GetImageSize(text, c_zoom);
    ww = (s.width() + 0.75*s.height()) / dpr + 2*msg_widget_->layout()->margin() + msg_widget_->layout()->spacing();
  }

  return c_zoom;
}

} // namespace quick_note
