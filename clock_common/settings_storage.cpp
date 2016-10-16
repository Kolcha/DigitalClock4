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

#include "settings_storage.h"

#include <QFile>
#include <QDataStream>

#ifdef PORTABLE_VERSION
#include <QApplication>
#include <QDir>

static QString GetConfigFileName()
{
  QDir app_dir(QApplication::applicationDirPath());
  return app_dir.filePath("settings.ini");
}

SettingsStorage::SettingsStorage(QObject* parent) :
  QObject(parent),
  storage_(GetConfigFileName(), QSettings::IniFormat)
{
}
#elif defined(Q_OS_OSX)
#include <QDir>
// macOS Sierra settings workaround, Qt BUG https://bugreports.qt.io/browse/QTBUG-56565
SettingsStorage::SettingsStorage(QObject* parent) :
  QObject(parent),
  storage_(QDir::home().filePath("Library/Preferences/digital_clock_4.ini"), QSettings::IniFormat)
{
}
#else
SettingsStorage::SettingsStorage(QObject* parent) : QObject(parent)
{
}
#endif

void SettingsStorage::SetValue(const QString& key, const QVariant& value)
{
  current_[key] = value;
}

QVariant SettingsStorage::GetValue(const QString& key, const QVariant& default_value)
{
  auto c_iter = current_.find(key);
  if (c_iter != current_.end()) return c_iter.value();

  if (isDeleted(key)) return default_value;

  // TODO: remove. Is it really needed?
  auto i_iter = imported_.find(key);
  if (i_iter != imported_.end()) return i_iter.value();

  QVariant value = storage_.value(key, default_value);
  current_[key] = value;
  return value;
}

void SettingsStorage::Remove(const QString& key)
{
  QStringList children = findKeyChildren(key, current_);
  for (auto& c : children) {
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
  for (auto& c : p_keys) {
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

void SettingsStorage::Export(const QString& filename)
{
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly)) return;
  QDataStream stream(&file);
  stream << current_;
  file.close();
}

void SettingsStorage::Import(const QString& filename)
{
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly)) return;
  QDataStream stream(&file);
  stream >> imported_;
  file.close();
  for (auto i = imported_.begin(); i != imported_.end(); ++i) {
    this->SetValue(i.key(), i.value());
  }
  emit reloaded();
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
  for (auto& key : keys) this->Revert(key);
  emit reloaded();
}

void SettingsStorage::Reset()
{
  // TODO: implement
}

bool SettingsStorage::isDeleted(const QString& key) const
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
  result.removeDuplicates();
  return result;
}
