#include "settings_storage.h"

#include "config_manager.h"

SettingsStorage::SettingsStorage(ConfigManager* manager, QObject *parent) :
  QObject(parent),
  manager_(manager)
{
  if (manager_) manager_->AddConfig(this);
}

SettingsStorage::~SettingsStorage()
{
  if (manager_) manager_->RemoveConfig(this);
}

const QVariant& SettingsStorage::GetValue(const QString& key, const QVariant& default_value)
{
  auto iter = current_values_.find(key);
  if (iter != current_values_.end()) return iter.value();
  default_values_[key] = default_value;
  QVariant& value = current_values_[key];
  value = settings_.value(key, default_value);
  return value;
}

void SettingsStorage::SetValue(const QString& key, const QVariant& value)
{
  current_values_[key] = value;
}

void SettingsStorage::Export(QSettings::SettingsMap* m) const
{
  for (auto iter = current_values_.begin(); iter != current_values_.end(); ++iter) {
    m->insert(iter.key(), iter.value());
  }
}

void SettingsStorage::Import(const QSettings::SettingsMap& m)
{
  for (auto iter = current_values_.begin(); iter != current_values_.end(); ++iter) {
    auto m_iter = m.find(iter.key());
    if (m_iter == m.end()) continue;
    *iter = m_iter.value();
  }
  emit reloaded();
}

void SettingsStorage::Load()
{
  for (auto iter = current_values_.begin(); iter != current_values_.end();) {
    QVariant value = settings_.value(iter.key());
    if (value.isValid()) {
      *iter = value;
      ++iter;
    } else {
      current_values_.erase(iter++);
    }
  }
  emit reloaded();
}

void SettingsStorage::Save()
{
  for (auto iter = current_values_.cbegin(); iter != current_values_.cend(); ++iter) {
    settings_.setValue(iter.key(), iter.value());
  }
}

void SettingsStorage::Reset()
{
  current_values_ = default_values_;
  emit reloaded();
}
