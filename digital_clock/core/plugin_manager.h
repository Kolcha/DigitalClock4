#ifndef DIGITAL_CLOCK_CORE_PLUGIN_MANAGER_H
#define DIGITAL_CLOCK_CORE_PLUGIN_MANAGER_H

#include "core/manager_base.h"

#include <QMap>
#include <QString>
#include <QList>

#include "iclock_plugin.h"
#include "core/plugin_info.h"

class QPluginLoader;
class QSystemTrayIcon;

namespace digital_clock {
namespace gui {
class ClockWidget;
}

namespace core {

class ClockSettings;

/*! Data to init plugins. */
struct TPluginData {
  ClockSettings* settings;                    /*!< clock settings object */
  gui::ClockWidget* window;                   /*!< clock widget */
  QSystemTrayIcon* tray;                      /*!< tray icon object */
};


/*!
 * @brief Plugin manager class.
 *
 * This class provides plugin management system. It loads/unloads plugins.
 * It distinguishes plugin types and int them with appropriate data.
 */
class PluginManager : public ManagerBase {
  Q_OBJECT

public:
  /*!
   * Constructor.
   * @param parent - parent object
   */
  explicit PluginManager(QObject* parent = 0);
  /*!
   * Set data for plugin initialization.
   * @param data - data to set. @see TPluginData
   */
  void SetInitData(const TPluginData& data);

signals:
  /*!
   * Signal to notify that plugins search was finished.
   * @param plugins - list of found plugins (pair: plugin name - is configurable)
   */
  void SearchFinished(const QList<QPair<TPluginInfo, bool> >& plugins);
  /*!
   * @brief Settings update signal.
   *
   * Special signal to establish communications between plugins (to track changes made by different
   * plugins). This signal is connected to SettingsListener() slot on each plugin and will emitted
   * when some 'settings' plugin changes any option.
   * @param option - changed option
   * @param value - new option's value
   * @note This signal is a part of internal PluginManager logic.
   * @see settings_keys.h, Options, IClockPlugin
   */
  void UpdateSettings(Option option, const QVariant& value);

public slots:
  /*!
   * List all available plugins. Signal @SearchFinished will emited with list of all plugins.
   */
  void ListAvailable();
  /*!
   * Load given plugins.
   * @param names - list of plugin names to load
   */
  void LoadPlugins(const QStringList& names);
  /*!
   * Unload given plugins. If list is empty - unload all loaded.
   * @param names - list of plugin names to unload
   */
  void UnloadPlugins(const QStringList& names = QStringList());
  /*!
   * Enable/disable given plugin.
   * @param name - plugin name
   * @param enable - true to enable, false to disable plugin
   */
  void EnablePlugin(const QString& name, bool enable);
  /*!
   * Call plugin settings dialog if available.
   * @param name - plugins name to configure
   */
  void ConfigurePlugin(const QString& name);

private:
  /*!
   * Load given plugin.
   * @param name - plugin name to load
   */
  void LoadPlugin(const QString& name);
  /*!
   * Unload given plugin.
   * @param name - plugin name to unload
   */
  void UnloadPlugin(const QString& name);
  /*!
   * @brief Init given plugin.
   *
   * Function determine plugin type and init it with appropriate data.
   * @param plugin - plugin to init
   * @param conneced - connect signal/slots
   */
  void InitPlugin(IClockPlugin* plugin, bool connected);

  QMap<QString, QString> available_;
  QMap<QString, QPluginLoader*> loaded_;
  TPluginData data_;
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_PLUGIN_MANAGER_H
