#ifndef SCHEDULE_SETTINGS_H
#define SCHEDULE_SETTINGS_H

#define APP_NAME   QString("Digital Clock")
#define ORG_NAME   QString("Nick Korotysh")
#define ROOT_KEY   QString("plugins/schedule/")

// schedule plugin settings keys
#define OPT_DATE_FORMAT    (ROOT_KEY + "date_format")
#define OPT_TASK_DELETE    (ROOT_KEY + "task_delete")

#include <QSettings>

inline void InitDefaults(QSettings::SettingsMap* defaults) {
  defaults->insert(OPT_DATE_FORMAT, (int)(Qt::SystemLocaleShortDate));
  defaults->insert(OPT_TASK_DELETE, true);
}

#endif // SCHEDULE_SETTINGS_H
