#include "plugin_settings.h"

#include "settings_storage.h"

PluginSettings::PluginSettings(SettingsStorage* backend, QObject* parent)
  : QObject(parent), backend_(backend), track_changes_(false) {
}

void PluginSettings::SetDefaultValues(const QSettings::SettingsMap& values) {
  default_map_ = values;
}

const QVariant& PluginSettings::GetOption(const QString& key) const {
  return backend_->GetValue(key, default_map_.find(key).value());   // TODO: track key name
}

void PluginSettings::Load() {
  for (auto iter = default_map_.begin(); iter != default_map_.end(); ++iter) {
    QVariant value = backend_->GetValue(iter.key(), iter.value());
    if (track_changes_) emit OptionChanged(iter.key(), value);
  }
}

void PluginSettings::Save() {
  backend_->Save();     // TODO: save only specific section
}

void PluginSettings::SetOption(const QString& key, const QVariant& value) {
  backend_->SetValue(key, value);       // TODO: add some key prefix (plugins/<plugin name>)
  if (track_changes_) emit OptionChanged(key, value);
}

void PluginSettings::TrackChanges(bool is_track) {
  track_changes_ = is_track;
}
