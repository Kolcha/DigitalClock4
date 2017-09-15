/*
    Digital Clock: date plugin
    Copyright (C) 2016-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef DATE_DATE_PLUGIN_H
#define DATE_DATE_PLUGIN_H

#include "widget_plugin_base.h"

#include <QTimeZone>

class QGridLayout;
class QLabel;

namespace date {

class DatePlugin : public ::plugin::WidgetPluginBase
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "date.json")

public:
  DatePlugin();

  void Init(const QMap<Option, QVariant>& current_settings) override;

public slots:
  void Configure() override;

  void SettingsListener(Option option, const QVariant& new_value) override;

protected:
  void InitSettingsDefaults(QSettings::SettingsMap* defaults) override;
  QWidget* InitWidget(QGridLayout* layout) override;
  void DisplayImage(const QImage& image) override;
  QString GetWidgetText() override;

private:
  QLabel* msg_label_;
  bool local_time_;
  QTimeZone time_zone_;
};

} // namespace date

#endif // DATE_DATE_PLUGIN_H
