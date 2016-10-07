/*
    Digital Clock: custom signal plugin
    Copyright (C) 2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef CUSTOM_SIGNAL_CUSTOM_SIGNAL_PLUGIN_H
#define CUSTOM_SIGNAL_CUSTOM_SIGNAL_PLUGIN_H

#include "iclock_plugin.h"

#include <QMediaPlayer>
#include <QList>
#include <QMap>

namespace custom_signal {

class SignalItem;

class CustomSignalPlugin : public IClockPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "custom_signal.json")
  Q_INTERFACES(IClockPlugin)

public:
  CustomSignalPlugin();

public slots:
  void Start();
  void Stop();

  void Configure();

  void TimeUpdateListener();

private slots:
  void SettingsUpdateListener(const QString& key, const QVariant& value);
  void onSignalTriggered();
  void onPlayerStateChanged(QMediaPlayer::State new_state);

private:
  enum SignalType {
    EveryHour,
    QuarterHour,
    CustomPeriod
  };

  bool started_;
  QMediaPlayer* player_;
  QMap<SignalType, SignalItem*> all_signals_;
  QList<SignalItem*> play_queue_;
};

} // namespace custom_signal

#endif // CUSTOM_SIGNAL_CUSTOM_SIGNAL_PLUGIN_H
