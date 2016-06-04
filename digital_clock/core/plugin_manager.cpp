#include "plugin_manager.h"

#include <QDir>
#include <QFile>
#include <QJsonObject>
#include <QApplication>
#include <QPluginLoader>

#include "iplugin_init.h"

#include "core/clock_settings.h"

#include "gui/clock_display.h"
#include "gui/clock_widget.h"

namespace digital_clock {
namespace core {

PluginManager::PluginManager(QObject *parent) : QObject(parent)
{
#ifdef Q_OS_OSX
  search_paths_.append(qApp->applicationDirPath() + "/../PlugIns");
#else
  search_paths_.append(qApp->applicationDirPath() + "/plugins");
#endif
#ifdef Q_OS_LINUX
  search_paths_.append("/usr/share/digital_clock/plugins");
  search_paths_.append("/usr/local/share/digital_clock/plugins");
  search_paths_.append(QDir::homePath() + "/.local/share/digital_clock/plugins");
#endif
}

void PluginManager::SetInitData(const TPluginData& data) {
  data_ = data;
}

void PluginManager::ListAvailable() {
  available_.clear();
  QList<QPair<TPluginInfo, bool> > plugins;
  for (auto& path : search_paths_) {
    QDir dir(path);
    QStringList files = dir.entryList(QDir::Files);
    for (auto& file : files) {
      QString abs_path = dir.filePath(file);
      QPluginLoader loader(abs_path);
      IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loader.instance());
      if (!plugin) continue;
      QJsonObject metadata = loader.metaData().value("MetaData").toObject();
      TPluginInfo info;
      QString c_name = metadata.value("name").toString();
      auto iter = std::find_if(plugins.cbegin(), plugins.cend(),
          [&] (const QPair<TPluginInfo, bool>& i) -> bool {
            return i.first.metadata[PI_NAME] == c_name;
          });
      if (iter == plugins.cend()) {
        info.metadata[PI_NAME] = c_name;
        info.metadata[PI_VERSION] = metadata.value("version").toString();
        info.metadata[PI_AUTHOR] = metadata.value("author").toString();
        info.metadata[PI_EMAIL] = metadata.value("email").toString();
        info.gui_info = plugin->GetInfo();
        available_[c_name] = abs_path;
        plugins.append(qMakePair(info, metadata.value("configurable").toBool()));
      }
      loader.unload();
    }
  }
  emit SearchFinished(plugins);
}

void PluginManager::LoadPlugins(const QStringList& names) {
  for (auto& name : names) LoadPlugin(name);
}

void PluginManager::UnloadPlugins(const QStringList& names) {
  if (!names.isEmpty()) {
    for (auto& name : names) UnloadPlugin(name);
  } else {
    QList<QString> plugins = loaded_.keys();
    for (auto& plugin : plugins) UnloadPlugin(plugin);
    for (auto i = tmp_loaded_.begin(); i != tmp_loaded_.end(); ++i) {
      i.value()->unload();
    }
    tmp_loaded_.clear();
  }
}

void PluginManager::EnablePlugin(const QString& name, bool enable) {
  enable ? LoadPlugin(name) : UnloadPlugin(name);
}

void PluginManager::ConfigurePlugin(const QString& name) {
  auto iter = loaded_.find(name);
  if (iter != loaded_.end()) {
    IClockPlugin* plugin = qobject_cast<IClockPlugin*>(iter.value()->instance());
    if (plugin) plugin->Configure();
  } else {
    auto iter = tmp_loaded_.find(name);
    if (iter != tmp_loaded_.end()) {
      QPluginLoader* loader = iter.value();
      IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loader->instance());
      if (plugin) plugin->Configure();
    } else {
      QString file = available_[name];
      if (!QFile::exists(file)) return;
      QPluginLoader* loader = new QPluginLoader(file, this);
      IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loader->instance());
      if (plugin) {
        plugin->InitSettings(data_.settings->GetBackend());
        InitPlugin(plugin, false);
        plugin->Configure();
      }
      tmp_loaded_[name] = loader;
    }
  }
}

void PluginManager::LoadPlugin(const QString& name) {
  if (loaded_.contains(name)) return;
  QString file = available_[name];
  if (!QFile::exists(file)) return;
  QPluginLoader* loader = new QPluginLoader(file, this);
  IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loader->instance());
  if (plugin) {
    QJsonObject metadata = loader->metaData().value("MetaData").toObject();
    if (metadata.value("configurable").toBool()) {
      plugin->InitSettings(data_.settings->GetBackend());
    }
    InitPlugin(plugin, true);
    plugin->Start();
    loaded_[name] = loader;
  }
}

void PluginManager::UnloadPlugin(const QString& name) {
  auto iter = loaded_.find(name);
  if (iter == loaded_.end()) return;
  QPluginLoader* loader = iter.value();
  Q_ASSERT(loader);
  IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loader->instance());
  if (plugin) {
    disconnect(data_.window->GetDisplay(), SIGNAL(ImageNeeded(QString)),
               plugin, SLOT(TimeUpdateListener()));
    plugin->Stop();
    loader->unload();
    loaded_.erase(iter);
  }
}

void PluginManager::InitPlugin(IClockPlugin* plugin, bool connected) {
  // connect slots which are common for all plugins
  if (connected) {
    connect(data_.window->GetDisplay(), SIGNAL(ImageNeeded(QString)),
            plugin, SLOT(TimeUpdateListener()));
    connect(this, SIGNAL(UpdateSettings(Option,QVariant)),
            plugin, SLOT(SettingsListener(Option,QVariant)));
  }
  // init settings plugins
  ISettingsPlugin* sp = qobject_cast<ISettingsPlugin*>(plugin);
  if (sp) {
    if (connected) {
      connect(sp, SIGNAL(OptionChanged(Option,QVariant)),
              data_.window, SLOT(ApplyOption(Option,QVariant)));
      connect(sp, SIGNAL(OptionChanged(Option,QVariant)),
              this, SIGNAL(UpdateSettings(Option,QVariant)));
    }
  }
  ISettingsPluginInit* spi = qobject_cast<ISettingsPluginInit*>(plugin);
  if (spi) spi->Init(data_.settings->GetSettings());
  // init tray plugins
  ITrayPluginInit* tpi = qobject_cast<ITrayPluginInit*>(plugin);
  if (tpi) tpi->Init(data_.tray);
  // init widget plugins
  IWidgetPluginInit* wpi = qobject_cast<IWidgetPluginInit*>(plugin);
  if (wpi) wpi->Init(data_.window);
}

} // namespace core
} // namespace digital_clock
