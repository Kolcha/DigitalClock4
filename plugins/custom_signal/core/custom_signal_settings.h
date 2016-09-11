#ifndef CUSTOM_SIGNAL_SETTINGS_H
#define CUSTOM_SIGNAL_SETTINGS_H

#include <QSettings>

namespace custom_signal {

#define OPT_EVERY_HOUR_ENABLED        "plugins/custom_signal/every_hour/enabled"
#define OPT_EVERY_HOUR_SIGNAL         "plugins/custom_signal/every_hour/signal"

#define OPT_QUARTER_HOUR_ENABLED      "plugins/custom_signal/quarter_hour/enabled"
#define OPT_QUARTER_HOUR_SIGNAL       "plugins/custom_signal/quarter_hour/signal"

#define OPT_CUSTOM_PERIOD_ENABLED     "plugins/custom_signal/custom/enabled"
#define OPT_CUSTOM_PERIOD_SIGNAL      "plugins/custom_signal/custom/signal"
#define OPT_CUSTOM_PERIOD_PERIOD      "plugins/custom_signal/custom/period"
#define OPT_CUSTOM_PERIOD_OFFSET      "plugins/custom_signal/custom/offset"

void InitDefaults(QSettings::SettingsMap* defaults);

} // namespace custom_signal

#endif // CUSTOM_SIGNAL_SETTINGS_H
