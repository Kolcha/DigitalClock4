#ifndef MOVE_CLOCK_SETTINGS_H
#define MOVE_CLOCK_SETTINGS_H

#include <QSettings>

namespace move_clock {
// move_clock plugin settings
#define OPT_X0        "plugins/move_clock/x0"
#define OPT_KX        "plugins/move_clock/kx"
#define OPT_AX        "plugins/move_clock/ax"
#define OPT_NX        "plugins/move_clock/nx"
#define OPT_Y0        "plugins/move_clock/y0"
#define OPT_KY        "plugins/move_clock/ky"
#define OPT_AY        "plugins/move_clock/ay"
#define OPT_NY        "plugins/move_clock/ny"
#define OPT_TIMEOUT   "plugins/move_clock/timeout"


inline void InitDefaults(QSettings::SettingsMap* defaults)
{
  defaults->insert(OPT_X0, 50);
  defaults->insert(OPT_KX, -2.0);
  defaults->insert(OPT_AX, 0);
  defaults->insert(OPT_NX, 0.01);
  defaults->insert(OPT_Y0, 20);
  defaults->insert(OPT_KY, 0.0);
  defaults->insert(OPT_AY, 0);
  defaults->insert(OPT_NY, 0.01);
  defaults->insert(OPT_TIMEOUT, 200);
}

} // namespace move_clock

#endif // MOVE_CLOCK_SETTINGS_H
