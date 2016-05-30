#include "settings_storage.h"

#include <QFile>
#include <QDataStream>

#ifdef PORTABLE_VERSION
#include <QApplication>
#include <QDir>

static QString GetConfigFileName() {
  QDir app_dir(QApplication::applicationDirPath());
  return app_dir.filePath("settings.ini");
}

SettingsStorage::SettingsStorage(QObject *parent) :
  QObject(parent),
  settings_(GetConfigFileName(), QSettings::IniFormat)
{
}
#else
SettingsStorage::SettingsStorage(QObject *parent) : QObject(parent)
{
}
#endif

const QVariant& SettingsStorage::GetValue(const QString& key, const QVariant& default_value)
{
  auto iter = current_values_.find(key);
  if (iter != current_values_.end()) return iter.value();
  default_values_[key] = default_value;
  QVariant& value = current_values_[key];
  value = isDeleted(key) ? default_value : settings_.value(key, default_value);
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

void SettingsStorage::Delete(const QString& key)
{
  QStringList children = findKeyChildren(key, current_values_);
  for (auto& c : children) {
    this->Delete(QString("%1/%2").arg(key, c));
  }
  removed_keys_.insert(key);
  current_values_.remove(key);
}

QStringList SettingsStorage::ListChildren(const QString& key)
{
  QStringList result = findKeyChildren(key, current_values_);
  QStringList p_keys;
  settings_.beginGroup(key);
  p_keys.append(settings_.childGroups());
  p_keys.append(settings_.childKeys());
  settings_.endGroup();
  for (auto& c : p_keys) {
    if (!isDeleted(QString("%1/%2").arg(key, c)) && !result.contains(c)) {
      result.append(c);
    }
  }
  return result;
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
  removed_keys_.clear();
  emit reloaded();
}

void SettingsStorage::Save()
{
  for (auto& key : removed_keys_) {
    settings_.remove(key);
    default_values_.remove(key);
  }
  removed_keys_.clear();

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

bool SettingsStorage::isDeleted(const QString& key)
{
  for (auto& i : removed_keys_) {
    if (key.mid(0, i.length()) == i) return true;
  }
  return false;
}

QStringList SettingsStorage::findKeyChildren(const QString& key, const QSettings::SettingsMap& m)
{
  QStringList result;
  QString path = key + "/";
  auto iter = m.lowerBound(path);
  if (iter == m.end()) return result;
  while (iter != m.end() && iter.key().mid(0, path.length()) == path) {
    int sep_pos = iter.key().indexOf('/', path.length());
    int len = -1;
    if (sep_pos != -1) len = sep_pos - path.length();
    result.append(iter.key().mid(path.length(), len));
    ++iter;
  }
  return result;
}
