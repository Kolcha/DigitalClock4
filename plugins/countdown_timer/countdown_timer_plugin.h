/*
    Digital Clock: countdown timer plugin
    Copyright (C) 2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef COUNTDOWN_TIMER_COUNTDOWN_TIMER_PLUGIN_H
#define COUNTDOWN_TIMER_COUNTDOWN_TIMER_PLUGIN_H

#include "widget_plugin_base.h"

class QGridLayout;
class QLabel;

namespace countdown_timer {

class CountdownTimer;

class CountdownTimerPlugin : public ::plugin::WidgetPluginBase
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "countdown_timer.json")

public:
  CountdownTimerPlugin();

public slots:
  void Start() override;
  void Stop() override;

  void Configure() override;

protected:
  void InitSettingsDefaults(QSettings::SettingsMap* defaults) override;
  QWidget* InitWidget(QGridLayout* layout) override;
  void DisplayImage(const QImage& image) override;
  QString GetWidgetText() override;

private slots:
  void InitTimer();
  void HandleTimeout();

private:
  QLabel* msg_label_;
  CountdownTimer* cd_timer_;
};

} // namespace countdown_timer

#endif // COUNTDOWN_TIMER_COUNTDOWN_TIMER_PLUGIN_H
