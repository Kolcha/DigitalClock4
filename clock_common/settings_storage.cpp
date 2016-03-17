#include "settings_storage.h"

#include <QFile>
#include <QDataStream>

SettingsStorage::SettingsStorage(QObject *parent) : QObject(parent)
{
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

void SettingsStorage::CommitValue(const QString& key, const QVariant& value)
{
  settings_.setValue(key, value);
}

void SettingsStorage::Remove(const QString& key)
{
  current_values_.remove(key);
  default_values_.remove(key);
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

void SettingsStorage::Export(const QString& filename)
{
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly)) return;
  QDataStream stream(&file);
  stream << current_values_;
  file.close();
}

void SettingsStorage::Import(const QString& filename)
{
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly)) return;
  QDataStream stream(&file);
  stream >> current_values_;
  file.close();
  emit reloaded();
}
