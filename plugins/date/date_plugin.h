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

#endif // DATE_DATE_PLUGIN_H
