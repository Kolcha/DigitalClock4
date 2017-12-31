/*
    Digital Clock: clock common library
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

/*!
 * @file settings_keys.h
 *
 * @brief All clock options and their possible values.
 *
 * You can find any clock option description, its possible values and value type in this file.
 * @see Option
 */

#ifndef SETTINGS_KEYS_H
#define SETTINGS_KEYS_H

/*! skin customization types */
enum class Customization {
  C_NONE,         /*!< do not use any customization, display skin as is */
  C_TEXTURING,    /*!< apply texture (solid color or custom image) to skin */
  C_COLORIZE      /*!< colorize skin without changing its texture */
};

/*! clock alignment value type */
enum CAlignment {
  A_LEFT = 1,     /*!< align left */
  A_RIGHT,        /*!< align right */
  A_CENTER        /*!< align center */
};

/*! clock settings keys */
/*! @note Comments format: option description, type, possible values */
enum Option {
  // clock settings
  OPT_OPACITY,                  /*!< clock opacity, qreal, 0..1 */
  OPT_STAY_ON_TOP,              /*!< stay on top flag, bool, true/false */
  OPT_TRANSP_FOR_INPUT,         /*!< "clicks passed through clock", bool, true/false */
  OPT_SEPARATOR_FLASH,          /*!< is separator flashes flag, bool, true/false */
  OPT_PLUGINS,                  /*!< plugins to load, QStringList, list of plugin names */
  OPT_TIME_FORMAT,              /*!< time format, QString, any valid time format string */
  OPT_ALIGNMENT,                /*!< clock alignment, CAlignment, A_LEFT/A_RIGHT/A_CENTER */
  OPT_BACKGROUND_ENABLED,       /*!< is background color will be used, bool, true/false */
  OPT_BACKGROUND_COLOR,         /*!< clock background color, QColor, any color */
  OPT_FULLSCREEN_DETECT,        /*!< detect fullscreen windows flag, bool, true/false */
  OPT_SHOW_ON_ALL_DESKTOPS,     /*!< show clock on all desktops flag, bool, true/false */
  OPT_DISPLAY_LOCAL_TIME,       /*!< display local time, bool, true/false */
  OPT_TIME_ZONE,                /*!< custom time zone, QString, IANA ID for the time zone */
  // skin settings
  OPT_SKIN_NAME,                /*!< skin name to use, QString, skin name */
  OPT_FONT,                     /*!< font to use, QFont, any available font */
  OPT_ZOOM,                     /*!< clock zoom, qreal, > 0 */
  OPT_COLOR,                    /*!< clock color (if texture is not used), QColor, any color */
  OPT_TEXTURE,                  /*!< path to texture file, QString, path to image file */
  OPT_TEXTURE_TYPE,             /*!< texture type, SkinDrawer::CustomizationType, CT_COLOR/CT_TEXTURE */
  OPT_TEXTURE_PER_ELEMENT,      /*!< apply texture per element flag, bool, true/false */
  OPT_TEXTURE_DRAW_MODE,        /*!< texture draw mode, SkinDrawer::DrawMode, DM_STRETCH/DM_TILE */
  OPT_CUSTOMIZATION,            /*!< customization type, Customization, C_NONE/C_TEXTURING/C_COLORIZE */
  OPT_SPACING,                  /*!< space between clock digits in pixels */
  OPT_COLORIZE_COLOR,           /*!< color for colorize effect, QColor, any color */
  OPT_COLORIZE_LEVEL,           /*!< colorize effect strength, qreal, 0..1 */
  // updater settings
  OPT_USE_AUTOUPDATE,           /*!< enable/disable autoupdate, bool, true/false */
  OPT_UPDATE_PERIOD,            /*!< how often to check for updates, qint64, days count */
  OPT_CHECK_FOR_BETA,           /*!< check for beta releases, bool, true/false */
  // misc settings
  OPT_CLOCK_URL_ENABLED,        /*!< enable/disable quick URL feature, bool, true/false */
  OPT_CLOCK_URL_STRING,         /*!< URL value for quick URL feature, QString, any URL */
  OPT_FULLSCREEN_IGNORE_LST,    /*!< list of fullscreen windows to ignore, QStringList, any strings */
  OPT_BETTER_STAY_ON_TOP,       /*!< use Qt::X11BypassWindowManagerHint, bool, true/false */
  OPT_SHOW_HIDE_ENABLED,        /*!< enable/disable 'Show/Hide' menu item, bool, true/false */
  OPT_EXPORT_STATE,             /*!< enable/disable state export with settings, bool, true/false */
  OPT_KEEP_ALWAYS_VISIBLE,      /*!< prevent out of range positions, bool, true/false */
  // window settings
  OPT_SHOW_WINDOW_BORDER,       /*!< highlight window border during configuration, bool, true/false */
  OPT_SNAP_TO_EDGES,            /*!< snap to edges/corners flag, bool, true/false */
  OPT_SNAP_THRESHOLD,           /*!< maximum distance when snap will be activated, int, 5..50 */
  OPT_SHOW_ON_ALL_MONITORS      /*!< show clock window on all monitors flag, bool, true/false */
};

#endif // SETTINGS_KEYS_H
