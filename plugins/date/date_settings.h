/*
    Digital Clock: date plugin
    Copyright (C) 2014-2019  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DATE_SETTINGS_H
#define DATE_SETTINGS_H

#include <QSettings>
#include <QLocale>

namespace date {

// date plugin settings
const char* const OPT_DATE_FORMAT_TYPE = "format_type";
const char* const OPT_DATE_FORMAT_STR  = "format_str";
const char* const OPT_DATE_FORMAT_INT  = "format_int";

enum class FormatType {
  FT_STR, FT_INT
};

inline void InitDefaults(QSettings::SettingsMap* defaults)
{
  defaults->insert(OPT_DATE_FORMAT_TYPE, static_cast<int>(FormatType::FT_INT));
  defaults->insert(OPT_DATE_FORMAT_STR, QLocale().dateFormat());
  defaults->insert(OPT_DATE_FORMAT_INT, Qt::DefaultLocaleLongDate);
}

} // namespace date

#endif // DATE_SETTINGS_H
