#include <QFont>
#include <QColor>
#include <QStringList>
#include <QLocale>
#include "settings_keys.h"

QVariant GetDefaultValue(Options opt) {
  switch (opt) {
    // clock settings
    case OPT_OPACITY:               return 0.75;
    case OPT_STAY_ON_TOP:           return true;
    case OPT_TRANSP_FOR_INPUT:      return false;
    case OPT_SEPARATOR_FLASH:       return true;
    case OPT_PLUGINS:               return QStringList("Test plugin");
    case OPT_TIME_FORMAT:           return QLocale::system().timeFormat();
    // skin settings
    case OPT_SKIN_NAME:             return QString("Electronic (default)");
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
