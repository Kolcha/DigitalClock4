#include "plugin_manager.h"

PluginManager::PluginManager(QObject *parent)
  : QObject(parent) {
}

void PluginManager::SetInitData(const TPluginData& data) {
  data_ = data;
}

void PluginManager::AddPluginsDir(const QDir& dir) {
  plugins_dirs_.append(dir);
}

void PluginManager::DelPluginsDir(const QDir& dir) {
  plugins_dirs_.removeOne(dir);
}

void PluginManager::ListAvailable() {
  available_.clear();
  for (auto& dir : plugins_dirs_) {
    QStringList files = dir.entryList(QDir::Files);
    for (auto& file : files) {
      QString abs_path = dir.filePath(file);
      QPluginLoader loader(abs_path);
      IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loader.instance());
      if (plugin) available_[plugin->GetInfo()[PI_NAME]] = abs_path;
      loader.unload();
    }
  }
  emit SearchFinished(available_.keys());
}

void PluginManager::LoadPlugins(const QStringList& files) {
  for (auto& file : files) {
    LoadPlugin(file);
  }
}

void PluginManager::EnablePlugin(const QString& name, bool enable) {
  QString file = available_[name];
  enable ? LoadPlugin(file) : UnloadPlugin(file);
}

void PluginManager::GetPluginInfo(const QString& name) {
}

void PluginManager::LoadPlugin(const QString& file) {
  QPluginLoader* loader = new QPluginLoader(file, this);
  IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loader->instance());
  if (plugin) {
    plugin->Init(data_);
    plugin->Start();
    loaded_[file] = loader;
  }
}

void PluginManager::UnloadPlugin(const QString& file) {
  QPluginLoader* loader = loaded_[file];
  IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loader->instance());
  if (plugin) {
    plugin->Stop();
    loader->unload();
    loaded_.remove(file);
  }
}
