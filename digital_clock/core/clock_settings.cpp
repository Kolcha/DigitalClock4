/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2013-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "clock_settings.h"

#include <QFont>
#include <QColor>
#include <QStringList>
#include <QTimeZone>
#include <QCoreApplication>

#include "settings_keys.h"

namespace digital_clock {
namespace core {

ClockSettings::ClockSettings(SettingsStorage* backend, QObject* parent) : ConfigBase(backend, parent)
{
}

QMap<Option, QVariant> ClockSettings::GetSettings() const
{
  QMap<Option, QVariant> all_settings;
  all_settings[OPT_OPACITY]               = GetValue(OPT_OPACITY);
  all_settings[OPT_STAY_ON_TOP]           = GetValue(OPT_STAY_ON_TOP);
  all_settings[OPT_TRANSP_FOR_INPUT]      = GetValue(OPT_TRANSP_FOR_INPUT);
  all_settings[OPT_SEPARATOR_FLASH]       = GetValue(OPT_SEPARATOR_FLASH);
  all_settings[OPT_PLUGINS]               = GetValue(OPT_PLUGINS);
  all_settings[OPT_TIME_FORMAT]           = GetValue(OPT_TIME_FORMAT);
  all_settings[OPT_ALIGNMENT]             = GetValue(OPT_ALIGNMENT);
  all_settings[OPT_BACKGROUND_ENABLED]    = GetValue(OPT_BACKGROUND_ENABLED);
  all_settings[OPT_BACKGROUND_COLOR]      = GetValue(OPT_BACKGROUND_COLOR);
  all_settings[OPT_FULLSCREEN_DETECT]     = GetValue(OPT_FULLSCREEN_DETECT);
  all_settings[OPT_SHOW_ON_ALL_DESKTOPS]  = GetValue(OPT_SHOW_ON_ALL_DESKTOPS);
  all_settings[OPT_DISPLAY_LOCAL_TIME]    = GetValue(OPT_DISPLAY_LOCAL_TIME);
  all_settings[OPT_TIME_ZONE]             = GetValue(OPT_TIME_ZONE);
  all_settings[OPT_SKIN_NAME]             = GetValue(OPT_SKIN_NAME);
  all_settings[OPT_FONT]                  = GetValue(OPT_FONT);
  all_settings[OPT_ZOOM]                  = GetValue(OPT_ZOOM);
  all_settings[OPT_COLOR]                 = GetValue(OPT_COLOR);
  all_settings[OPT_TEXTURE]               = GetValue(OPT_TEXTURE);
  all_settings[OPT_TEXTURE_TYPE]          = GetValue(OPT_TEXTURE_TYPE);
  all_settings[OPT_TEXTURE_PER_ELEMENT]   = GetValue(OPT_TEXTURE_PER_ELEMENT);
  all_settings[OPT_TEXTURE_DRAW_MODE]     = GetValue(OPT_TEXTURE_DRAW_MODE);
  all_settings[OPT_CUSTOMIZATION]         = GetValue(OPT_CUSTOMIZATION);
  all_settings[OPT_SPACING]               = GetValue(OPT_SPACING);
  all_settings[OPT_COLORIZE_COLOR]        = GetValue(OPT_COLORIZE_COLOR);
  all_settings[OPT_COLORIZE_LEVEL]        = GetValue(OPT_COLORIZE_LEVEL);
  all_settings[OPT_USE_AUTOUPDATE]        = GetValue(OPT_USE_AUTOUPDATE);
  all_settings[OPT_UPDATE_PERIOD]         = GetValue(OPT_UPDATE_PERIOD);
  all_settings[OPT_CHECK_FOR_BETA]        = GetValue(OPT_CHECK_FOR_BETA);
  all_settings[OPT_CLOCK_URL_ENABLED]     = GetValue(OPT_CLOCK_URL_ENABLED);
  all_settings[OPT_CLOCK_URL_STRING]      = GetValue(OPT_CLOCK_URL_STRING);
  all_settings[OPT_FULLSCREEN_IGNORE_LST] = GetValue(OPT_FULLSCREEN_IGNORE_LST);
  all_settings[OPT_BETTER_STAY_ON_TOP]    = GetValue(OPT_BETTER_STAY_ON_TOP);
  all_settings[OPT_SHOW_HIDE_ENABLED]     = GetValue(OPT_SHOW_HIDE_ENABLED);
  all_settings[OPT_EXPORT_STATE]          = GetValue(OPT_EXPORT_STATE);
  all_settings[OPT_KEEP_ALWAYS_VISIBLE]   = GetValue(OPT_KEEP_ALWAYS_VISIBLE);
  all_settings[OPT_ONLY_ONE_INSTANCE]     = GetValue(OPT_ONLY_ONE_INSTANCE);
  all_settings[OPT_SHOW_WINDOW_BORDER]    = GetValue(OPT_SHOW_WINDOW_BORDER);
  all_settings[OPT_SNAP_TO_EDGES]         = GetValue(OPT_SNAP_TO_EDGES);
  all_settings[OPT_SNAP_THRESHOLD]        = GetValue(OPT_SNAP_THRESHOLD);
  all_settings[OPT_SHOW_ON_ALL_MONITORS]  = GetValue(OPT_SHOW_ON_ALL_MONITORS);
  return all_settings;
}

QString ClockSettings::GetKey(const int id) const
{
  switch (static_cast<Option>(id)) {
    // clock settings
    case OPT_OPACITY:               return "clock/opacity";
    case OPT_STAY_ON_TOP:           return "clock/stay_on_top";
    case OPT_TRANSP_FOR_INPUT:      return "clock/transp_for_input";
    case OPT_SEPARATOR_FLASH:       return "clock/separator_flash";
    case OPT_PLUGINS:               return "clock/plugins";
    case OPT_TIME_FORMAT:           return "clock/time_format";
    case OPT_ALIGNMENT:             return "clock/alignment";
    case OPT_BACKGROUND_ENABLED:    return "clock/background_enabled";
    case OPT_BACKGROUND_COLOR:      return "clock/background_color";
    case OPT_FULLSCREEN_DETECT:     return "clock/fullscreen_detect";
    case OPT_SHOW_ON_ALL_DESKTOPS:  return "clock/show_on_all_desktops";
    case OPT_DISPLAY_LOCAL_TIME:    return "clock/local_time";
    case OPT_TIME_ZONE:             return "clock/time_zone";
    // skin settings
    case OPT_SKIN_NAME:             return "skin/name";
    case OPT_FONT:                  return "skin/font";
    case OPT_ZOOM:                  return "skin/zoom";
    case OPT_COLOR:                 return "skin/color";
    case OPT_TEXTURE:               return "skin/texture";
    case OPT_TEXTURE_TYPE:          return "skin/texture_type";
    case OPT_TEXTURE_PER_ELEMENT:   return "skin/texture_per_element";
    case OPT_TEXTURE_DRAW_MODE:     return "skin/texture_draw_mode";
    case OPT_CUSTOMIZATION:         return "skin/customization";
    case OPT_SPACING:               return "skin/spacing";
    case OPT_COLORIZE_COLOR:        return "skin/colorize_color";
    case OPT_COLORIZE_LEVEL:        return "skin/colorize_level";
    // updater settings
    case OPT_USE_AUTOUPDATE:        return "updater/autoupdate";
    case OPT_UPDATE_PERIOD:         return "updater/update_period";
    case OPT_CHECK_FOR_BETA:        return "updater/check_for_beta";
    // misc settings
    case OPT_CLOCK_URL_ENABLED:     return "misc/clock_url_enabled";
    case OPT_CLOCK_URL_STRING:      return "misc/clock_url_string";
    case OPT_FULLSCREEN_IGNORE_LST: return "misc/fullscreen_ignore_list";
    case OPT_BETTER_STAY_ON_TOP:    return "misc/better_stay_on_top";
    case OPT_SHOW_HIDE_ENABLED:     return "misc/show_hide_enabled";
    case OPT_EXPORT_STATE:          return "misc/export_state";
    case OPT_KEEP_ALWAYS_VISIBLE:   return "window/always_visible";
    case OPT_ONLY_ONE_INSTANCE:     return "app/single_instance";
    // window settings
    case OPT_SHOW_WINDOW_BORDER:    return "window/show_border";
    case OPT_SNAP_TO_EDGES:         return "window/snap_to_edges";
    case OPT_SNAP_THRESHOLD:        return "window/snap_threshold";
    case OPT_SHOW_ON_ALL_MONITORS:  return "window/show_on_all_monitors";
  }
  Q_ASSERT(false);
  return QString();
}

QVariant ClockSettings::GetDefaultValue(const int id) const
{
  switch (static_cast<Option>(id)) {
    // clock settings
    case OPT_OPACITY:               return 0.75;
    case OPT_STAY_ON_TOP:           return true;
    case OPT_TRANSP_FOR_INPUT:      return false;
    case OPT_SEPARATOR_FLASH:       return true;
    case OPT_PLUGINS:               return QStringList();
    case OPT_TIME_FORMAT:           return QString();
    case OPT_ALIGNMENT:             return static_cast<int>(CAlignment::A_LEFT);
    case OPT_BACKGROUND_ENABLED:    return false;
    case OPT_BACKGROUND_COLOR:      return QVariant::fromValue<QColor>(Qt::white);
    case OPT_FULLSCREEN_DETECT:     return false;
    case OPT_SHOW_ON_ALL_DESKTOPS:  return false;
    case OPT_DISPLAY_LOCAL_TIME:    return true;
    case OPT_TIME_ZONE:             return QString::fromLatin1(QTimeZone::systemTimeZoneId());
    // skin settings
    case OPT_SKIN_NAME:             return QString("Electronic (default)");
    case OPT_FONT:                  return QFont();
    case OPT_ZOOM:                  return 1.25;
    case OPT_COLOR:                 return QColor(0, 170, 255);
    case OPT_TEXTURE:               return QString();
    case OPT_TEXTURE_TYPE:          return 1;
    case OPT_TEXTURE_PER_ELEMENT:   return false;
    case OPT_TEXTURE_DRAW_MODE:     return 0;
    case OPT_CUSTOMIZATION:         return 1;
    case OPT_SPACING:               return 4;
    case OPT_COLORIZE_COLOR:        return QColor(0, 85, 255);
    case OPT_COLORIZE_LEVEL:        return 0.8;
    // updater settings
    case OPT_USE_AUTOUPDATE:        return true;
    case OPT_UPDATE_PERIOD:         return 3;
    case OPT_CHECK_FOR_BETA:        return *(QCoreApplication::applicationVersion().rbegin()) == '+';
    // misc settings
    case OPT_CLOCK_URL_ENABLED:     return false;
    case OPT_CLOCK_URL_STRING:      return QString("http://digitalclock4.sourceforge.net/");
    case OPT_FULLSCREEN_IGNORE_LST: return QStringList() << "Chrome_WidgetWin_0" << "CEF-OSR-WIDGET";
    case OPT_BETTER_STAY_ON_TOP:    return false;
    case OPT_SHOW_HIDE_ENABLED:     return false;
    case OPT_EXPORT_STATE:          return true;
    case OPT_KEEP_ALWAYS_VISIBLE:   return false;
    case OPT_ONLY_ONE_INSTANCE:     return false;
    // window settings
    case OPT_SHOW_WINDOW_BORDER:    return true;
    case OPT_SNAP_TO_EDGES:         return true;
    case OPT_SNAP_THRESHOLD:        return 15;
    case OPT_SHOW_ON_ALL_MONITORS:  return false;
  }
  return QVariant();
}

} // namespace core
} // namespace digital_clock
