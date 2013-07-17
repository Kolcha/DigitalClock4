#ifndef SETTINGS_KEYS_H
#define SETTINGS_KEYS_H

#include <QMap>
#include <QString>
#include <QVariant>
#include <QPoint>
#include <QColor>
#include <QStringList>

enum Options {
  // clock settings
  OPT_OPACITY,
  OPT_STAY_ON_TOP,
  OPT_TRANSP_FOR_INPUT,
  OPT_SEPARATOR_FLASH,
  OPT_PLUGINS,
  // skin settings
  OPT_SKIN_NAME,
  OPT_ZOOM,
  OPT_COLOR,
  OPT_TEXTURE,
  OPT_TEXTURE_PER_ELEMENT,
  OPT_TEXTURE_DRAW_MODE,
  OPT_USE_TEXTURE
};


inline void GetOptionsKeys(QMap<Options, QString>& opt_map) {
  // clock settings
  opt_map[OPT_OPACITY] = "clock/opacity";
  opt_map[OPT_STAY_ON_TOP] = "clock/stay_on_top";
  opt_map[OPT_TRANSP_FOR_INPUT] = "clock/transp_for_input";
  opt_map[OPT_SEPARATOR_FLASH] = "clock/separator_flash";
  opt_map[OPT_PLUGINS] = "clock/plugins";
  // skin settings
  opt_map[OPT_SKIN_NAME] = "skin/name";
  opt_map[OPT_ZOOM] = "skin/zoom";
  opt_map[OPT_COLOR] = "skin/color";
  opt_map[OPT_TEXTURE] = "skin/texture";
  opt_map[OPT_TEXTURE_PER_ELEMENT] = "skin/texture_per_element";
  opt_map[OPT_TEXTURE_DRAW_MODE] = "skin/texture_draw_mode";
  opt_map[OPT_USE_TEXTURE] = "skin/use_texture";
}

inline QVariant GetDefaultValue(Options opt) {
  switch (opt) {
    // clock settings
    case OPT_OPACITY:               return 0.75;
    case OPT_STAY_ON_TOP:           return false;
    case OPT_TRANSP_FOR_INPUT:      return false;
    case OPT_SEPARATOR_FLASH:       return true;
    case OPT_PLUGINS:               return QStringList("Test plugin");
    // skin settings
    case OPT_SKIN_NAME:             return QString("Electronic (default)");
    case OPT_ZOOM:                  return 1.25;
    case OPT_COLOR:                 return QVariant::fromValue<QColor>(Qt::blue);
    case OPT_TEXTURE:               return QString();
    case OPT_TEXTURE_PER_ELEMENT:   return false;
    case OPT_TEXTURE_DRAW_MODE:     return 0;
    case OPT_USE_TEXTURE:           return false;
    default:                        return QVariant();
  }
}

#endif // SETTINGS_KEYS_H
