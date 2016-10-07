/*
    Digital Clock: clock common library
    Copyright (C) 2013-2016  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "plugin_settings.h"

#include "settings_storage.h"

PluginSettings::PluginSettings(SettingsStorage* backend, QObject* parent) :
  QObject(parent),
  backend_(backend), track_changes_(false)
{
  connect(backend_, &SettingsStorage::reloaded, this, &PluginSettings::Reload);
}

PluginSettings::~PluginSettings()
{
  for (auto iter = default_map_.begin(); iter != default_map_.end(); ++iter) {
    backend_->Forget(iter.key());
  }
}

void PluginSettings::SetDefaultValues(const QSettings::SettingsMap& values)
{
  default_map_ = values;
}

QVariant PluginSettings::GetOption(const QString& key) const
{
  auto iter = current_map_.find(key);
  if (iter != current_map_.end()) return iter.value();
  return backend_->GetValue(key, default_map_.find(key).value());   // TODO: track key name
}

void PluginSettings::Load()
{
  current_map_.clear();
  for (auto iter = default_map_.begin(); iter != default_map_.end(); ++iter) {
    backend_->Revert(iter.key());
    QVariant value = backend_->GetValue(iter.key(), iter.value());
    if (track_changes_) emit OptionChanged(iter.key(), value);
  }
}

void PluginSettings::Save()
{
  for (auto iter = current_map_.begin(); iter != current_map_.end(); ++iter) {
    backend_->Commit(iter.key());
  }
}

void PluginSettings::SetOption(const QString& key, const QVariant& value)
{
  current_map_[key] = value;       // TODO: add some key prefix (plugins/<plugin name>)
  backend_->SetValue(key, value);
  if (track_changes_) emit OptionChanged(key, value);
}

void PluginSettings::TrackChanges(bool is_track)
{
  track_changes_ = is_track;
}

void PluginSettings::Reload()
{
  bool old_value = track_changes_;
  track_changes_ = true;
  Load();
  track_changes_ = old_value;
}
