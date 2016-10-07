/*
    Digital Clock: date plugin
    Copyright (C) 2014-2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "date_plugin.h"

#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QDate>

#include "plugin_settings.h"

#include "date_settings.h"
#include "gui/settings_dialog.h"

namespace date {

DatePlugin::DatePlugin() : msg_label_(nullptr)
{
  InitTranslator(QLatin1String(":/date/date_"));
  info_.display_name = tr("Date");
  info_.description = tr("Allows to display current date under clock.");
  InitIcon(":/date/icon.svg");
  plg_name_ = QString("date");
}

void DatePlugin::Configure()
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

void DatePlugin::InitSettingsDefaults(QSettings::SettingsMap* defaults)
{
  InitDefaults(defaults);
}

QWidget* DatePlugin::InitWidget(QGridLayout* layout)
{
  msg_label_ = new QLabel();
  msg_label_->setAlignment(Qt::AlignCenter);
  layout->addWidget(msg_label_, layout->rowCount(), 0, 1, layout->columnCount());
  return msg_label_;
}

void DatePlugin::DisplayImage(const QImage& image)
{
  msg_label_->setPixmap(QPixmap::fromImage(image));
}

QString DatePlugin::GetWidgetText()
{
  QString date;
  QDate d_date = QDate::currentDate();

  switch (static_cast<FormatType>(settings_->GetOption(OPT_DATE_FORMAT_TYPE).toInt())) {
    case FormatType::FT_INT:
      date = d_date.toString(static_cast<Qt::DateFormat>(settings_->GetOption(OPT_DATE_FORMAT_INT).toInt()));
      break;

    case FormatType::FT_STR:
    {
      QString format = settings_->GetOption(OPT_DATE_FORMAT_STR).toString();

      int prev = -1;
      bool is_start = false;
      while (true) {
        int pos = format.indexOf('\'', prev + 1);
        QString substr = format.mid(prev + 1, pos - prev - 1);
        if (is_start) {
          int s_pos = substr.indexOf("\\n");
          if (s_pos != -1) substr.replace(s_pos, 2, '\n');
          is_start = false;
        } else {
          // parse 'WW' or 'W' -- week number
          int s_pos = substr.indexOf("WW", 0, Qt::CaseInsensitive);
          if (s_pos != -1) {
            substr.replace(s_pos, 2, QString("%1").arg(d_date.weekNumber(), 2, 10, QChar('0')));
          } else {
            s_pos = substr.indexOf('W', 0, Qt::CaseInsensitive);
            if (s_pos != -1) substr.replace(s_pos, 1, QString::number(d_date.weekNumber()));
          }
          // parse 'J' -- day of year
          s_pos = substr.indexOf('J', 0, Qt::CaseSensitive);
          if (s_pos != -1) substr.replace(s_pos, 1, QString::number(d_date.dayOfYear()));
          is_start = true;
        }
        format.replace(prev + 1, pos != -1 ? pos - prev - 1 : format.length() - prev - 1, substr);
        pos = format.indexOf('\'', prev + 1);
        if (pos == -1) break;
        prev = pos;
      }

      date = d_date.toString(format);
      break;
    }
  }
  return date;
}

} // namespace date
