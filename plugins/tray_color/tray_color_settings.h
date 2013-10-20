#ifndef TRAY_COLOR_SETTINGS_H
#define TRAY_COLOR_SETTINGS_H

// alarm plugin settings keys
#define OPT_TRAY_COLOR           "tray_color/color"

#include <QSettings>

inline void InitDefaults(QSettings::SettingsMap* defaults) {
  defaults->insert(OPT_TRAY_COLOR, QVariant::fromValue<QColor>(Qt::blue));
}

#endif // TRAY_COLOR_SETTINGS_H
