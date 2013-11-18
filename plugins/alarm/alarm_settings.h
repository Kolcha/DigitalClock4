#ifndef ALARM_SETTINGS_H
#define ALARM_SETTINGS_H

// alarm plugin settings keys
#define OPT_ENABLED           "plugins/alarm/enabled"
#define OPT_TIME              "plugins/alarm/time"
#define OPT_SIGNAL_TYPE       "plugins/alarm/signal_type"
#define OPT_FILENAME          "plugins/alarm/filename"
#define OPT_STREAM_URL        "plugins/alarm/stream_url"
#define OPT_VOLUME            "plugins/alarm/volume"
#define OPT_SHOW_NOTIFY       "plugins/alarm/show_notify"
#define OPT_NOTIFY_TEXT       "plugins/alarm/notify_text"

#include <QSettings>
#include <QTime>

enum SignalType { ST_FILE, ST_STREAM };

inline void InitDefaults(QSettings::SettingsMap* defaults) {
  defaults->insert(OPT_ENABLED, false);
  defaults->insert(OPT_TIME, QTime());
  defaults->insert(OPT_SIGNAL_TYPE, (int)ST_FILE);
  defaults->insert(OPT_FILENAME, QString());
  defaults->insert(OPT_STREAM_URL, QString());
  defaults->insert(OPT_VOLUME, 50);
  defaults->insert(OPT_SHOW_NOTIFY, false);
  defaults->insert(OPT_NOTIFY_TEXT, QString());
}

#endif // ALARM_SETTINGS_H
