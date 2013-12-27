#include <QTime>
#include <QSound>
#include "hour_signal.h"

namespace hour_signal {

HourSignal::HourSignal() : last_hour_(-1) {
  InitTranslator(QLatin1String(":/hour_signal/hour_signal_"));
  info_.display_name = tr("Every hour signal");
  info_.description = tr("Plays short signal every hour.");
}

void HourSignal::TimeUpdateListener() {
  int current_hour = QTime::currentTime().toString("H").toInt();
  if (current_hour != last_hour_ && last_hour_ != -1) QSound::play(":/hour_signal/hour_signal.wav");
  last_hour_ = current_hour;
}

} // namespace hour_signal
