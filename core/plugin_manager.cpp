#include <QFile>
#include "clock_settings.h"
#include "../gui/main_window.h"
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
      TPluginInfo info;
      plugin->GetInfo(&info);
      if (plugin) available_[info[PI_NAME]] = abs_path;
      loader.unload();
    }
  }
  emit SearchFinished(available_.keys());
}

void PluginManager::LoadPlugins(const QStringList& names) {
  for (auto& name : names) LoadPlugin(name);
}

void PluginManager::EnablePlugin(const QString& name, bool enable) {
  enable ? LoadPlugin(name) : UnloadPlugin(name);
}

void PluginManager::GetPluginInfo(const QString& name) {
  QString file = available_[name];
  QPluginLoader loader(file);
  IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loader.instance());
  TPluginInfo info;
  plugin->GetInfo(&info);
  emit InfoGot(info);
  loader.unload();
}

void PluginManager::LoadPlugin(const QString& name) {
  QString file = available_[name];
  if (!QFile::exists(file)) return;
  QPluginLoader* loader = new QPluginLoader(file, this);
  IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loader->instance());
  if (plugin) {
    InitPlugin(plugin);
    plugin->Start();
    loaded_[name] = loader;
  }
}

void PluginManager::UnloadPlugin(const QString& name) {
  QPluginLoader* loader = loaded_[name];
  IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loader->instance());
  if (plugin) {
    plugin->Stop();
    loader->unload();
    loaded_.remove(name);
  }
}

void PluginManager::InitPlugin(IClockPlugin* plugin) {
  ISettingsPlugin* sp = qobject_cast<ISettingsPlugin*>(plugin);
  if (sp) {
    sp->Init(data_.settings->GetSettings());
    connect(sp, SIGNAL(OptionChanged(Options,QVariant)),
            data_.window, SLOT(SettingsListener(Options,QVariant)));
  }
}
