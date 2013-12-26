#ifndef POWER_OFF_SETTINGS_H
#define POWER_OFF_SETTINGS_H

#include <QSettings>
#include <QTime>

namespace power_off {

// power_off plugin settings
#define OPT_TIME    "plugins/power_off/time"

inline void InitDefaults(QSettings::SettingsMap* defaults) {
  defaults->insert(OPT_TIME, QTime());
}

} // namespace power_off

#endif // POWER_OFF_SETTINGS_H
