#include "clock_settings.h"

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
