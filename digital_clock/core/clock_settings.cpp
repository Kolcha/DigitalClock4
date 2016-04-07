#include "clock_settings.h"

#include <QFont>
#include <QColor>
#include <QStringList>

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
    case OPT_PLUGINS:               return QStringList("Test plugin");
    case OPT_TIME_FORMAT:           return QString();
    case OPT_ALIGNMENT:             return static_cast<int>(CAlignment::A_LEFT);
    case OPT_BACKGROUND_ENABLED:    return false;
    case OPT_BACKGROUND_COLOR:      return QVariant::fromValue<QColor>(Qt::white);
    // skin settings
    case OPT_SKIN_NAME:             return QString("Electronic (default)");
    case OPT_FONT:                  return QFont();
    case OPT_ZOOM:                  return 1.25;
    case OPT_COLOR:                 return QVariant::fromValue<QColor>(Qt::blue);
    case OPT_TEXTURE:               return QString();
    case OPT_TEXTURE_TYPE:          return 1;
    case OPT_TEXTURE_PER_ELEMENT:   return false;
    case OPT_TEXTURE_DRAW_MODE:     return 0;
    case OPT_CUSTOMIZATION:         return 1;
    case OPT_SPACING:               return 4;
    case OPT_COLORIZE_COLOR:        return QVariant::fromValue<QColor>(Qt::darkCyan);
    case OPT_COLORIZE_LEVEL:        return 0.8;
    // updater settings
    case OPT_USE_AUTOUPDATE:        return true;
    case OPT_UPDATE_PERIOD:         return 3;
    case OPT_CHECK_FOR_BETA:        return false;
  }
  return QVariant();
}

} // namespace core
} // namespace digital_clock
