#include "clock_settings.h"

void GetOptionsKeys(QMap<Options, QString>& opt_map) {
  // clock settings
  opt_map[OPT_OPACITY]              = "clock/opacity";
  opt_map[OPT_STAY_ON_TOP]          = "clock/stay_on_top";
  opt_map[OPT_TRANSP_FOR_INPUT]     = "clock/transp_for_input";
  opt_map[OPT_SEPARATOR_FLASH]      = "clock/separator_flash";
  opt_map[OPT_PLUGINS]              = "clock/plugins";
  opt_map[OPT_TIME_FORMAT]          = "clock/time_format";
  // skin settings
  opt_map[OPT_SKIN_NAME]            = "skin/name";
  opt_map[OPT_FONT]                 = "skin/font";
  opt_map[OPT_ZOOM]                 = "skin/zoom";
  opt_map[OPT_COLOR]                = "skin/color";
  opt_map[OPT_TEXTURE]              = "skin/texture";
  opt_map[OPT_TEXTURE_PER_ELEMENT]  = "skin/texture_per_element";
  opt_map[OPT_TEXTURE_DRAW_MODE]    = "skin/texture_draw_mode";
  opt_map[OPT_CUSTOMIZATION]        = "skin/customization";
}

ClockSettings::ClockSettings(QObject* parent)
  : QObject(parent) {
  GetOptionsKeys(keys_);
  track_change_ = false;
}

const QVariant& ClockSettings::GetOption(Options opt) {
  return values_[opt];
}

const QMap<Options, QVariant>&ClockSettings::GetSettings() {
  return values_;
}

void ClockSettings::Load() {
  for (auto i = keys_.begin(); i != keys_.end(); ++i) {
    QVariant value = settings_.value(i.value(), GetDefaultValue(i.key()));
    values_[i.key()] = value;
    if (track_change_) emit OptionChanged(i.key(), value);
  }
}

void ClockSettings::Save() {
  for (auto i = values_.begin(); i != values_.end(); ++i) {
    settings_.setValue(keys_[i.key()], i.value());
  }
}

void ClockSettings::SetOption(Options opt, const QVariant& value) {
  values_[opt] = value;
  if (track_change_) emit OptionChanged(opt, value);
}

void ClockSettings::TrackChanges(bool set) {
  track_change_ = set;
}