#include "quarter_hour_signal.h"

#include <QTime>
#include <QSound>

namespace quarter_hour_signal {

QuarterHourSignal::QuarterHourSignal() : last_minute_(-1) {
  InitTranslator(QLatin1String(":/quarter_hour_signal/quarter_hour_signal_"));
  info_.display_name = tr("Quarter of an hour signal");
  info_.description = tr("Chiming on every quarter of an hour.");
  InitIcon(":/quarter_hour_signal/icon.svg");
}

void QuarterHourSignal::TimeUpdateListener() {
  int cur_min = QTime::currentTime().minute();
  if (cur_min == 0 || cur_min == 15 || cur_min == 30 || cur_min == 45) {
    if (cur_min != last_minute_) {
      QSound::play(":/quarter_hour_signal/signal.wav");
    }
  }
  last_minute_ = cur_min;
}

} // namespace quarter_hour_signal
