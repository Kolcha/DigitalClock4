#ifndef ALARM_SETTINGS_H
#define ALARM_SETTINGS_H

// alarm plugin settings keys
#define OPT_ENABLED           "alarm_plugin/enabled"
#define OPT_TIME              "alarm_plugin/time"
#define OPT_SIGNAL_TYPE       "alarm_plugin/signal_type"
#define OPT_FILENAME          "alarm_plugin/filename"
#define OPT_STREAM_URL        "alarm_plugin/stream_url"
#define OPT_SHOW_NOTIFY       "alarm_plugin/show_notify"
#define OPT_NOTIFY_TEXT       "alarm_plugin/notify_text"

#include <QSettings>
#include <QTime>

enum SignalType { ST_FILE, ST_STREAM };

inline void InitDefaults(QSettings::SettingsMap* defaults) {
  defaults->insert(OPT_ENABLED, false);
  defaults->insert(OPT_TIME, QTime());
  defaults->insert(OPT_SIGNAL_TYPE, (int)ST_FILE);
  defaults->insert(OPT_FILENAME, QString());
  defaults->insert(OPT_STREAM_URL, QString());
  defaults->insert(OPT_SHOW_NOTIFY, false);
  defaults->insert(OPT_NOTIFY_TEXT, QString());
}

#endif // ALARM_SETTINGS_H
