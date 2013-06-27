#ifndef SETTINGS_KEYS_H
#define SETTINGS_KEYS_H

#include <QMap>
#include <QString>

enum Options {
  // clock settings
  OPT_OPACITY,
  OPT_STAY_ON_TOP,
  OPT_TRANSP_FOR_INPUT,
  OPT_SEPARATOR_FLASH,
  OPT_POSITION,
  // skin settings
  OPT_SKIN_NAME,
  OPT_ZOOM,
  OPT_COLOR,
  OPT_TEXTURE,
  OPT_TEXTURE_PER_ELEMENT,
  OPT_TEXTURE_DRAW_MODE
};


inline void GetOptionsKeys(QMap<Options, QString>& opt_map) {
  // clock settings
  opt_map[OPT_OPACITY] = "clock/opacity";
  opt_map[OPT_STAY_ON_TOP] = "clock/stay_on_top";
  opt_map[OPT_TRANSP_FOR_INPUT] = "clock/transp_for_input";
  opt_map[OPT_SEPARATOR_FLASH] = "clock/separator_flash";
  opt_map[OPT_POSITION] = "clock/position";
  // skin settings
  opt_map[OPT_SKIN_NAME] = "skin/name";
  opt_map[OPT_ZOOM] = "skin/zoom";
  opt_map[OPT_COLOR] = "skin/color";
  opt_map[OPT_TEXTURE] = "skin/texture";
  opt_map[OPT_TEXTURE_PER_ELEMENT] = "skin/texture_per_element";
  opt_map[OPT_TEXTURE_DRAW_MODE] = "skin/texture_draw_mode";
}

#endif // SETTINGS_KEYS_H
