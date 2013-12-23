#ifndef TRAY_COLOR_SETTINGS_H
#define TRAY_COLOR_SETTINGS_H

#include <QSettings>

namespace tray_color {

// alarm plugin settings keys
#define OPT_TRAY_COLOR           "plugins/tray_color/color"

inline void InitDefaults(QSettings::SettingsMap* defaults) {
  defaults->insert(OPT_TRAY_COLOR, QVariant::fromValue<QColor>(Qt::blue));
}

} // namespace tray_color

#endif // TRAY_COLOR_SETTINGS_H
