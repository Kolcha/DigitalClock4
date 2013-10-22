#ifndef SETTINGS_KEYS_H
#define SETTINGS_KEYS_H

#include "clock_common_global.h"
#include <QVariant>

/*! clock settings keys */
/*! @note Comments format: option description, type, possible values */
enum Options {
  // clock settings
  OPT_OPACITY,                  /*!< clock opacity, qreal, 0..1 */
  OPT_STAY_ON_TOP,              /*!< stay on top flag, bool, true/false */
  OPT_TRANSP_FOR_INPUT,         /*!< "clicks passed through clock", bool, true/false */
  OPT_SEPARATOR_FLASH,          /*!< is separator flashes flag, bool, true/false */
  OPT_DISPLAY_AM_PM,            /*!< display AM/PM symbol, bool, true/false */
  OPT_PLUGINS,                  /*!< plugins to load, QStringList, list of plugin names */
  // skin settings
  OPT_USE_SKIN,                 /*!< use skin instead of font, bool, true/false */
  OPT_SKIN_NAME,                /*!< skin name to use, QString, skin name */
  OPT_USE_FONT,                 /*!< use font instead of skin, bool, true/false */
  OPT_FONT,                     /*!< font to use, QFont, any available font */
  OPT_ZOOM,                     /*!< clock zoom, qreal, > 0 */
  OPT_COLOR,                    /*!< clock color (if texture is not used), QColor, any color */
  OPT_TEXTURE,                  /*!< path to texture file, QString, path to image file */
  OPT_TEXTURE_PER_ELEMENT,      /*!< apply texture per element flag, bool, true/false */
  OPT_TEXTURE_DRAW_MODE,        /*!< texture draw mode, @see DrawMode, DM_STRETCH/DM_TILE */
  OPT_USE_TEXTURE               /*!< use texture instead of solid color flag, bool, true/false */
};

/*! Get default value for specified option.
 * @param opt - needed option
 * @return default value as QVariant, see Options description for type information
 */
CLOCK_COMMON_EXPORT QVariant GetDefaultValue(Options opt);

#endif // SETTINGS_KEYS_H
