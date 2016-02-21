#include "config_manager.h"

#include <QSettings>
#include <QFile>
#include <QDataStream>

#include "settings_storage.h"

ConfigManager::ConfigManager(QObject *parent) : QObject(parent)
{
}

void ConfigManager::AddConfig(SettingsStorage* obj)
{
  config_objs_.insert(obj);
}

void ConfigManager::RemoveConfig(SettingsStorage* obj)
{
  config_objs_.remove(obj);
}

void ConfigManager::Export(const QString& filename) const
{
  QSettings::SettingsMap all_settings;
  for (auto& c : config_objs_) c->Export(&all_settings);
  if (all_settings.empty()) return;

  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly)) return;
  QDataStream stream(&file);
  stream << all_settings;
  file.close();
}

void ConfigManager::Import(const QString& filename) const
{
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly)) return;
  QDataStream stream(&file);
  QSettings::SettingsMap all_settings;
  stream >> all_settings;
  file.close();

  if (all_settings.empty()) return;
  for (auto& c : config_objs_) c->Import(all_settings);
}
