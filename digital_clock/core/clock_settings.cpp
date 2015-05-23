#include "clock_settings.h"

namespace digital_clock {
namespace core {

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
  opt_map[OPT_TEXTURE_TYPE]         = "skin/texture_type";
  opt_map[OPT_TEXTURE_PER_ELEMENT]  = "skin/texture_per_element";
  opt_map[OPT_TEXTURE_DRAW_MODE]    = "skin/texture_draw_mode";
  opt_map[OPT_CUSTOMIZATION]        = "skin/customization";
  opt_map[OPT_SPACING]              = "skin/spacing";
  // updater settings
  opt_map[OPT_USE_AUTOUPDATE]       = "updater/autoupdate";
  opt_map[OPT_UPDATE_PERIOD]        = "updater/update_period";
  opt_map[OPT_CHECK_FOR_BETA]       = "updater/check_for_beta";
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

void ClockSettings::ExportSettings(QSettings::SettingsMap* settings) {
  Q_ASSERT(settings);
  Q_ASSERT(keys_.size() == values_.size());
  auto k_iter = keys_.cbegin();
  auto v_iter = values_.cbegin();
  for (; k_iter != keys_.cend(); ++k_iter, ++v_iter) {
    settings->insert(k_iter.value(), v_iter.value());
  }
  emit SettingsExported();
}

void ClockSettings::ImportSettings(const QSettings::SettingsMap& settings) {
  for (auto i = keys_.cbegin(); i != keys_.cend(); ++i) {
    values_[i.key()] = settings.value(i.value(), GetDefaultValue(i.key()));
  }
  emit SettingsImported();
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

void ClockSettings::EmitSettings() {
  for (auto i = values_.begin(); i != values_.end(); ++i) {
    emit OptionChanged(i.key(), i.value());
  }
}

void ClockSettings::LoadDefaults() {
  for (auto i = keys_.begin(); i != keys_.end(); ++i) {
    QVariant value = GetDefaultValue(i.key());
    values_[i.key()] = value;
    if (track_change_) emit OptionChanged(i.key(), value);
  }
  emit SettingsImported();
}

} // namespace core
} // namespace digital_clock
