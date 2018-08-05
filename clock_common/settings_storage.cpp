/*
    Digital Clock: clock common library
    Copyright (C) 2016-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "settings_storage.h"

#include "config_serialization.h"

SettingsStorage::SettingsStorage(QObject* parent) : QObject(parent)
{
}

SettingsStorage::SettingsStorage(const QString& filename, QObject* parent) :
  QObject(parent),
  storage_(filename, QSettings::IniFormat)
{
}

void SettingsStorage::SetValue(const QString& key, const QVariant& value)
{
  current_[key] = value;
}

QVariant SettingsStorage::GetValue(const QString& key, const QVariant& default_value) const
{
  auto c_iter = current_.find(key);
  if (c_iter != current_.end()) return c_iter.value();

  if (isDeleted(key)) return default_value;

  auto i_iter = imported_.find(key);
  if (i_iter != imported_.end()) return i_iter.value();

  QVariant value = storage_.value(key, default_value);
  if (default_value.isValid()) value.convert(default_value.type());
  current_[key] = value;
  return value;
}

void SettingsStorage::Remove(const QString& key)
{
  QStringList children = findKeyChildren(key, current_);
  for (auto& c : qAsConst(children)) {
    this->Remove(QString("%1/%2").arg(key, c));
  }
  removed_keys_.insert(key);
  current_.remove(key);
}

QStringList SettingsStorage::ListChildren(const QString& key)
{
  QStringList result = findKeyChildren(key, current_);
  QStringList p_keys;
  storage_.beginGroup(key);
  p_keys.append(storage_.childGroups());
  p_keys.append(storage_.childKeys());
  storage_.endGroup();
  for (auto& c : qAsConst(p_keys)) {
    if (!isDeleted(QString("%1/%2").arg(key, c)) && !result.contains(c)) {
      result.append(c);
    }
  }
  return result;
}

void SettingsStorage::Commit(const QString& key)
{
  if (isDeleted(key)) {
    QString min_parent;
    for (auto iter = removed_keys_.begin(); iter != removed_keys_.end();) {
      if (key.mid(0, iter->length()) == *iter) {
        if (min_parent.isEmpty()) min_parent = *iter;
        if (iter->length() < min_parent.length()) min_parent = *iter;
        removed_keys_.erase(iter++);
      } else {
        ++iter;
      }
    }
    storage_.remove(min_parent);
  }
  auto iter = current_.find(key);
  if (iter == current_.end()) return;
  storage_.setValue(iter.key(), iter.value());
}

void SettingsStorage::Revert(const QString& key)
{
  if (isDeleted(key)) {
    for (auto iter = removed_keys_.begin(); iter != removed_keys_.end();) {
      if (iter->mid(0, key.length()) == key) {
        removed_keys_.erase(iter++);
      } else {
        ++iter;
      }
    }
  }
  auto i_iter = imported_.find(key);
  if (i_iter != imported_.end()) {
    current_[key] = i_iter.value();
  } else {
    current_.remove(key);
  }
}

void SettingsStorage::Forget(const QString& key)
{
  current_.remove(key);
}

void SettingsStorage::Export(const QString& filename) const
{
  QString ext = filename.mid(filename.lastIndexOf('.') + 1).toLower();
  if (ext == "dcs") config::ExportDCS(filename, current_);
  if (ext == "ini") config::ExportINI(filename, current_);
}

void SettingsStorage::Import(const QString& filename)
{
  QString ext = filename.mid(filename.lastIndexOf('.') + 1).toLower();
  imported_.clear();
  if (ext == "dcs") config::ImportDCS(filename, imported_);
  if (ext == "ini") config::ImportINI(filename, imported_);
  for (auto i = imported_.cbegin(); i != imported_.cend(); ++i)
    this->SetValue(i.key(), i.value());
  if (!imported_.isEmpty()) emit reloaded();
}

void SettingsStorage::Accept()
{
  for (auto& key : imported_.keys()) this->Commit(key);
  imported_.clear();
}

void SettingsStorage::Reject()
{
  QStringList keys = imported_.keys();
  imported_.clear();
  for (auto& key : qAsConst(keys)) this->Revert(key);
  emit reloaded();
}

void SettingsStorage::Reset()
{
  // TODO: implement
}

bool SettingsStorage::isDeleted(const QString& key) const
{
  for (auto& i : qAsConst(removed_keys_)) {
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
  result.removeDuplicates();
  return result;
}
