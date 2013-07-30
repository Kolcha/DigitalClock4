#include "hour_signal.h"

void HourSignal::GetInfo(TPluginInfo* info) {
  info->insert(PI_NAME, "Every hour signal");
  info->insert(PI_TYPE, "clock");
  info->insert(PI_VERSION, "2.0");
  info->insert(PI_AUTHOR, "Nick Korotysh");
  info->insert(PI_EMAIL, "nick.korotysh@gmail.com");
  info->insert(PI_COMMENT, "Plays short signal every hour.");
}

void HourSignal::TimeUpdateListener(const QString& current_time) {
}
