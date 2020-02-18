/*
    Digital Clock: timetracker plugin
    Copyright (C) 2018-2020  Nick Korotysh <nick.korotysh@gmail.com>

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
#ifndef TIMETRACKER_TIMETRACKER_PLUGIN_H
#define TIMETRACKER_TIMETRACKER_PLUGIN_H

#include "widget_plugin_base.h"

namespace timetracker {

class Timetracker;

class TimetrackerPlugin : public ::plugin::WidgetPluginBase
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "timetracker.json")

public:
  TimetrackerPlugin();

public slots:
  void Start() override;
  void Stop() override;

protected:
  QWidget* InitWidget(QGridLayout* layout) override;
  void DisplayImage(QWidget* widget, const QImage& image) override;
  QString GetWidgetText() override;

private slots:
  void onWidgetClicked();

private:
  Timetracker* tracker_;
};

} // namespace timetracker

#endif // TIMETRACKER_TIMETRACKER_PLUGIN_H
