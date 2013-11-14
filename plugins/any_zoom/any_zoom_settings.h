#ifndef ANY_ZOOM_SETTINGS_H
#define ANY_ZOOM_SETTINGS_H

// any zoom plugin settings keys
#define OPT_CURRENT_ZOOM        "plugins/any_zoom/zoom"

#include <QSettings>

inline void InitDefaults(QSettings::SettingsMap* defaults) {
  defaults->insert(OPT_CURRENT_ZOOM, 425);
}

#endif // ANY_ZOOM_SETTINGS_H
