#ifndef ANY_ZOOM_SETTINGS_H
#define ANY_ZOOM_SETTINGS_H

// any zoom plugin settings keys
#define OPT_CURRENT_ZOOM        "any_zoom/current"
#define OPT_PREVIOUS_ZOOM       "any_zoom/previous"

#include <QSettings>

inline void InitDefaults(QSettings::SettingsMap* defaults) {
  defaults->insert(OPT_CURRENT_ZOOM, 4.25);
  defaults->insert(OPT_PREVIOUS_ZOOM, 4.0);
}

#endif // ANY_ZOOM_SETTINGS_H
