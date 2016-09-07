#ifndef POWER_OFF_SETTINGS_H
#define POWER_OFF_SETTINGS_H

#include <QSettings>
#include <QTime>

namespace power_off {

// power_off plugin settings
#define OPT_TIME    "plugins/power_off/time"
#define OPT_FORCE   "plugins/power_off/force"

inline void InitDefaults(QSettings::SettingsMap* defaults)
{
  defaults->insert(OPT_TIME, QTime());
  defaults->insert(OPT_FORCE, false);
}

} // namespace power_off

#endif // POWER_OFF_SETTINGS_H
