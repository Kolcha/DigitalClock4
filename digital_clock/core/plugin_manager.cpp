#include "plugin_manager.h"

#include <QFile>
#include <QJsonObject>
#include <QPluginLoader>

#include "iplugin_init.h"

#include "core/clock_settings.h"

#include "gui/clock_display.h"
#include "gui/clock_widget.h"

namespace digital_clock {
namespace core {

PluginManager::PluginManager(QObject *parent) : ManagerBase(parent) {
}

void PluginManager::SetInitData(const TPluginData& data) {
  data_ = data;
}

void PluginManager::ExportPluginsSettings(QMap<QString, QSettings::SettingsMap>* settings) {
  Q_ASSERT(settings);
  for (auto i = available_.cbegin(); i != available_.cend(); ++i) {
    QSettings::SettingsMap& plugin_settings = (*settings)[i.key()];
    if (loaded_.contains(i.key())) {
      IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loaded_[i.key()]->instance());
      if (plugin) plugin->ExportSettings(&plugin_settings);
    } else {
      QString file = i.value();
      if (!QFile::exists(file)) continue;
      QPluginLoader* loader = new QPluginLoader(file, this);
      IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loader->instance());
      if (plugin) {
        InitPlugin(plugin, false);
        plugin->ExportSettings(&plugin_settings);
      }
    }
  }
}

void PluginManager::ImportPluginsSettings(const QMap<QString, QSettings::SettingsMap>& settings) {
  for (auto i = available_.cbegin(); i != available_.cend(); ++i) {
    auto s_iter = settings.find(i.key());
    if (s_iter == settings.cend()) continue;
    const QSettings::SettingsMap& plugin_settings = s_iter.value();
    if (loaded_.contains(i.key())) {
      IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loaded_[i.key()]->instance());
      if (plugin) plugin->ImportSettings(plugin_settings);
    } else {
      QString file = i.value();
      if (!QFile::exists(file)) continue;
      QPluginLoader* loader = new QPluginLoader(file, this);
      IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loader->instance());
      if (plugin) {
        InitPlugin(plugin, false);
        plugin->ImportSettings(plugin_settings);
      }
    }
  }
}

void PluginManager::ListAvailable() {
  available_.clear();
  QList<QPair<TPluginInfo, bool> > plugins;
  for (auto& dir : search_dirs_) {
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
  }
}

void PluginManager::EnablePlugin(const QString& name, bool enable) {
  enable ? LoadPlugin(name) : UnloadPlugin(name);
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
    disconnect(data_.settings, SIGNAL(OptionChanged(Option,QVariant)),
               plugin, SLOT(SettingsListener(Option,QVariant)));
    disconnect(data_.window->GetDisplay(), SIGNAL(ImageNeeded(QString)),
               plugin, SLOT(TimeUpdateListener()));
    plugin->Stop();
    loader->unload();
    loaded_.remove(name);
  }
}

void PluginManager::InitPlugin(IClockPlugin* plugin, bool connected) {
  // connect slots which are common for all plugins
  if (connected) {
    connect(data_.settings, SIGNAL(OptionChanged(Option,QVariant)),
            plugin, SLOT(SettingsListener(Option,QVariant)));
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
