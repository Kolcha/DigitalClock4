#ifndef DATE_SETTINGS_H
#define DATE_SETTINGS_H

#include <QSettings>
#include <QLocale>

namespace date {

// date plugin settings
#define OPT_DATE_FORMAT_TYPE  "plugins/date/format_type"
#define OPT_DATE_FORMAT_STR   "plugins/date/format_str"
#define OPT_DATE_FORMAT_INT   "plugins/date/format_int"
#define OPT_USE_CLOCK_FONT    "plugins/date/clock_font"
#define OPT_DATE_FONT         "plugins/date/font"
#define OPT_ZOOM_MODE         "plugins/date/zoom_mode"

enum class FormatType {
  FT_STR, FT_INT
};

enum class ZoomMode {
  ZM_NOT_ZOOM, ZM_AUTOSIZE, ZM_CLOCK_ZOOM
};

inline void InitDefaults(QSettings::SettingsMap* defaults) {
  defaults->insert(OPT_DATE_FORMAT_TYPE, (int)FormatType::FT_INT);
  defaults->insert(OPT_DATE_FORMAT_STR, QLocale().dateFormat());
  defaults->insert(OPT_DATE_FORMAT_INT, Qt::DefaultLocaleLongDate);
  defaults->insert(OPT_USE_CLOCK_FONT, true);
  defaults->insert(OPT_DATE_FONT, QFont());
  defaults->insert(OPT_ZOOM_MODE, (int)ZoomMode::ZM_AUTOSIZE);
}

} // namespace date

#endif // DATE_SETTINGS_H
