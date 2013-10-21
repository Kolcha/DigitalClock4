#ifndef SETTINGS_KEYS_H
#define SETTINGS_KEYS_H

#include "clock_common_global.h"
#include <QVariant>

// TODO: add options description
enum Options {
  // clock settings
  OPT_OPACITY,
  OPT_STAY_ON_TOP,
  OPT_TRANSP_FOR_INPUT,
  OPT_SEPARATOR_FLASH,
  OPT_DISPLAY_AM_PM,
  OPT_PLUGINS,
  // skin settings
  OPT_USE_SKIN,
  OPT_SKIN_NAME,
  OPT_USE_FONT,
  OPT_FONT,
  OPT_ZOOM,
  OPT_COLOR,
  OPT_TEXTURE,
  OPT_TEXTURE_PER_ELEMENT,
  OPT_TEXTURE_DRAW_MODE,
  OPT_USE_TEXTURE
};

CLOCK_COMMON_EXPORT QVariant GetDefaultValue(Options opt);

#endif // SETTINGS_KEYS_H
