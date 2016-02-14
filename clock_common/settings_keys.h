/*!
 * @file settings_keys.h
 *
 * @brief All clock options and their possible values.
 *
 * You can find any clock option description, its possible values and value type in this file.
 * @see Options, GetDefaultValue()
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
  A_RIGHT         /*!< align right */
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
  OPT_ALIGNMENT,                /*!< clock alignment, CAlignment, A_LEFT/A_RIGHT */
  // skin settings
  OPT_SKIN_NAME,                /*!< skin name to use, QString, skin name */
  OPT_FONT,                     /*!< font to use, QFont, any available font */
  OPT_ZOOM,                     /*!< clock zoom, qreal, > 0 */
  OPT_COLOR,                    /*!< clock color (if texture is not used), QColor, any color */
  OPT_TEXTURE,                  /*!< path to texture file, QString, path to image file */
  OPT_TEXTURE_TYPE,             /*!< texture type, SkinDrawer::CustomizationType, CT_COLOR/CT_TEXTURE */
  OPT_TEXTURE_PER_ELEMENT,      /*!< apply texture per element flag, bool, true/false */
  OPT_TEXTURE_DRAW_MODE,        /*!< texture draw mode, SkinDrawer::DrawMode, DM_STRETCH/DM_TILE */
  OPT_CUSTOMIZATION,            /*!< customization type, Customization,
                                     C_NONE/C_TEXTURING/C_COLORIZE */
  OPT_SPACING,                  /*!< space between clock digits in pixels */
  OPT_COLORIZE_COLOR,           /*!< color for colorize effect, QColor, any color */
  OPT_COLORIZE_LEVEL,           /*!< colorize effect strength, qreal, 0..1 */

  // updater settings
  OPT_USE_AUTOUPDATE,           /*!< enable/disable autoupdate, bool, true/false */
  OPT_UPDATE_PERIOD,            /*!< how often to check for updates, qint64, days count */
  OPT_CHECK_FOR_BETA            /*!< check for beta releases, bool, true/false */
};

#endif // SETTINGS_KEYS_H
