#include <QFont>
#include <QColor>
#include <QStringList>
#include "settings_keys.h"

QVariant GetDefaultValue(Options opt) {
  switch (opt) {
    // clock settings
    case OPT_OPACITY:               return 0.75;
    case OPT_STAY_ON_TOP:           return false;
    case OPT_TRANSP_FOR_INPUT:      return false;
    case OPT_SEPARATOR_FLASH:       return true;
    case OPT_DISPLAY_AM_PM:         return false;
    case OPT_PLUGINS:               return QStringList("Test plugin");
    // skin settings
    case OPT_USE_SKIN:              return true;
    case OPT_SKIN_NAME:             return QString("Electronic (default)");
    case OPT_USE_FONT:              return false;
    case OPT_FONT:                  return QFont();
    case OPT_ZOOM:                  return 1.25;
    case OPT_COLOR:                 return QVariant::fromValue<QColor>(Qt::blue);
    case OPT_TEXTURE:               return QString();
    case OPT_TEXTURE_PER_ELEMENT:   return false;
    case OPT_TEXTURE_DRAW_MODE:     return 0;
    case OPT_CUSTOMIZATION:         return 1;
    default:                        return QVariant();
  }
}
