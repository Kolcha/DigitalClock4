#ifndef DATE_SETTINGS_H
#define DATE_SETTINGS_H

#include <QSettings>
#include <QLocale>

namespace date {

// date plugin settings
#define OPT_DATE_FORMAT_TYPE  "plugins/date/format_type"
#define OPT_DATE_FORMAT_STR   "plugins/date/format_str"
#define OPT_DATE_FORMAT_INT   "plugins/date/format_int"

enum class FormatType {
  FT_STR, FT_INT
};

inline void InitDefaults(QSettings::SettingsMap* defaults) {
  defaults->insert(OPT_DATE_FORMAT_TYPE, (int)FormatType::FT_INT);
  defaults->insert(OPT_DATE_FORMAT_STR, QLocale().dateFormat());
  defaults->insert(OPT_DATE_FORMAT_INT, Qt::DefaultLocaleLongDate);
}

} // namespace date

#endif // DATE_SETTINGS_H
