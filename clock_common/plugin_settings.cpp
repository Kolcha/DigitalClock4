#include "plugin_settings.h"

PluginSettings::PluginSettings(const QString& org_name, const QString& app_name, QObject* parent)
  : QObject(parent), settings_(org_name, app_name), track_changes_(false) {
}

void PluginSettings::SetDefaultValues(const QSettings::SettingsMap& values) {
  settings_map_ = values;
}

const QVariant& PluginSettings::GetOption(const QString& key) const {
  return settings_map_.find(key).value();
}

void PluginSettings::Load() {
  for (auto iter = settings_map_.begin(); iter != settings_map_.end(); ++iter) {
    settings_map_[iter.key()] = settings_.value(iter.key(), iter.value());
    if (track_changes_) emit OptionChanged(iter.key(), iter.value());
  }
}

void PluginSettings::Save() {
  for (auto iter = settings_map_.begin(); iter != settings_map_.end(); ++iter) {
    settings_.setValue(iter.key(), iter.value());
  }
}

void PluginSettings::SetOption(const QString& key, const QVariant& value) {
  settings_map_[key] = value;
  if (track_changes_) emit OptionChanged(key, value);
}

void PluginSettings::TrackChanges(bool is_track) {
  track_changes_ = is_track;
}
