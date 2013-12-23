#ifndef ANY_ZOOM_SETTINGS_H
#define ANY_ZOOM_SETTINGS_H

#include <QSettings>

namespace any_zoom {

// any zoom plugin settings keys
#define OPT_CURRENT_ZOOM        "plugins/any_zoom/zoom"

inline void InitDefaults(QSettings::SettingsMap* defaults) {
  defaults->insert(OPT_CURRENT_ZOOM, 425);
}

} // namespace any_zoom

#endif // ANY_ZOOM_SETTINGS_H
