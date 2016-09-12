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
