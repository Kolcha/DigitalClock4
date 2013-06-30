#include "clock_settings.h"

ClockSettings::ClockSettings(QObject* parent)
  : QObject(parent) {
  GetOptionsKeys(keys_);
}

void ClockSettings::Load() {
  for (auto i = keys_.begin(); i != keys_.end(); ++i) {
    QVariant value = settings_.value(i.value(), GetDefaultValue(i.key()));
    values_[i.key()] = value;
    emit OptionChanged(i.key(), value);
  }
}

void ClockSettings::Save() {
  for (auto i = values_.begin(); i != values_.end(); ++i) {
    settings_.setValue(keys_[i.key()], i.value());
  }
}

void ClockSettings::SetOption(Options opt, const QVariant& value) {
  values_[opt] = value;
  emit OptionChanged(opt, value);
}
