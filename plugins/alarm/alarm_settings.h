#ifndef ALARM_SETTINGS_H
#define ALARM_SETTINGS_H

#define OPT_ENABLED           "alarm_plugin/enabled"
#define OPT_TIME              "alarm_plugin/time"
#define OPT_SIGNAL            "alarm_plugin/signal"
#define OPT_SHOW_NOTIFY       "alarm_plugin/show_notify"
#define OPT_NOTIFY_TEXT       "alarm_plugin/notify_text"

#include <QSettings>
#include <QTime>
inline void InitDefaults(QSettings::SettingsMap* defaults) {
  defaults->insert(OPT_ENABLED, false);
  defaults->insert(OPT_TIME, QTime());
  defaults->insert(OPT_SIGNAL, QString("default"));
  defaults->insert(OPT_SHOW_NOTIFY, false);
  defaults->insert(OPT_NOTIFY_TEXT, QString());
}

#endif // ALARM_SETTINGS_H
