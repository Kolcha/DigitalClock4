#include <QSound>
#include <QRegExp>
#include "hour_signal.h"

void HourSignal::GetInfo(TPluginInfo* info) {
  info->insert(PI_NAME, "Every hour signal");
  info->insert(PI_TYPE, "clock");
  info->insert(PI_VERSION, "2.0");
  info->insert(PI_AUTHOR, "Nick Korotysh");
  info->insert(PI_EMAIL, "nick.korotysh@gmail.com");
  info->insert(PI_COMMENT, "Plays short signal every hour.");
  info->insert(PI_CONFIG, "false");
}
// TODO: fix get time logic [any time format feature]
void HourSignal::TimeUpdateListener(const QString& current_time) {
  int current_hour = current_time.mid(0, current_time.indexOf(QRegExp("[ :]"))).toInt();
  if (current_hour != last_hour_ && last_hour_ != -1) QSound::play(":/hour_signal.wav");
  last_hour_ = current_hour;
}
