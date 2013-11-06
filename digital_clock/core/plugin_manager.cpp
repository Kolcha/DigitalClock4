#include <QFile>
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
  QList<QPair<QString, bool> > plugins;
  for (auto& dir : plugins_dirs_) {
    QStringList files = dir.entryList(QDir::Files);
    for (auto& file : files) {
      QString abs_path = dir.filePath(file);
      QPluginLoader loader(abs_path);
      IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loader.instance());
      if (plugin) {
        TPluginInfo info;
        plugin->GetInfo(&info);
        if (plugin) {
          available_[info[PI_NAME]] = abs_path;
          plugins.append(qMakePair(info[PI_NAME], QVariant(info[PI_CONFIG]).toBool()));
        }
        loader.unload();
      }
    }
  }
  emit SearchFinished(plugins);
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

void PluginManager::ConfigurePlugin(const QString& name) {
  if (loaded_.contains(name)) {
    IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loaded_[name]->instance());
    if (plugin) plugin->Configure();
  } else {
    QString file = available_[name];
    if (!QFile::exists(file)) return;
    QPluginLoader* loader = new QPluginLoader(file, this);
    IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loader->instance());
    if (plugin) {
      InitPlugin(plugin, false);
      plugin->Configure();
    }
  }
}

void PluginManager::LoadPlugin(const QString& name) {
  QString file = available_[name];
  if (!QFile::exists(file)) return;
  QPluginLoader* loader = new QPluginLoader(file, this);
  IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loader->instance());
  if (plugin) {
    InitPlugin(plugin, true);
    plugin->Start();
    loaded_[name] = loader;
  }
}

void PluginManager::UnloadPlugin(const QString& name) {
  QPluginLoader* loader = loaded_[name];
  if (!loader) return;
  IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loader->instance());
  if (plugin) {
    plugin->Stop();
    loader->unload();
    loaded_.remove(name);
  }
}

void PluginManager::InitPlugin(IClockPlugin* plugin, bool connected) {
  // connect slots which are common for all plugins
  if (connected) {
    connect(data_.settings, SIGNAL(OptionChanged(Options,QVariant)),
            plugin, SLOT(SettingsListener(Options,QVariant)));
    connect(data_.clock, SIGNAL(ImageNeeded(QString)),
            plugin, SLOT(TimeUpdateListener(QString)));
  }
  // init settings plugins
  ISettingsPlugin* sp = qobject_cast<ISettingsPlugin*>(plugin);
  if (sp) {
    sp->Init(data_.settings->GetSettings(), data_.window);
    if (connected) {
      connect(sp, SIGNAL(OptionChanged(Options,QVariant)),
              data_.window, SLOT(SettingsListener(Options,QVariant)));
    }
  }
  // init tray plugins
  ITrayPlugin* tp = qobject_cast<ITrayPlugin*>(plugin);
  if (tp) {
    tp->Init(data_.tray, data_.window);
  }
  // init widget plugins
  IWidgetPlugin* wp = qobject_cast<IWidgetPlugin*>(plugin);
  if (wp) {
    wp->Init(data_.window);
  }
}