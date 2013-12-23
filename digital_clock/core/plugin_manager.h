#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <QMap>
#include <QDir>
#include <QString>
#include <QList>
#include <QPluginLoader>
#include "iclock_plugin.h"
#include "plugin_info.h"

class ClockSettings;
class DigitalClock;

namespace digital_clock {
class MainWindow;
}

/*! Data to init plugins. */
struct TPluginData {
  ClockSettings* settings;   /*!< clock settings object */
  DigitalClock* clock;       /*!< clock widget */
  digital_clock::MainWindow* window;  /*!< main window instance */
  QSystemTrayIcon* tray;     /*!< tray icon object */
};


/*!
 * @brief Plugin manager class.
 *
 * This class provides plugin management system. It loads/unloads plugins.
 * It distinguishes plugin types and int them with appropriate data.
 */
class PluginManager : public QObject {
  Q_OBJECT
public:
  /*!
   * Constructor.
   * @param parent - parent object
   */
  explicit PluginManager(QObject* parent = 0);
  /*!
   * Destructor.
   */
  ~PluginManager();
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

public slots:
  /*!
   * Add plugins search directory.
   * @param dir - search directory
   */
  void AddPluginsDir(const QDir& dir);
  /*!
   * Remove plugins search directory.
   * @param dir - directory to remove
   */
  void DelPluginsDir(const QDir& dir);
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

  QList<QDir> plugins_dirs_;
  QMap<QString, QString> available_;
  QMap<QString, QPluginLoader*> loaded_;
  TPluginData data_;
};

#endif // PLUGIN_MANAGER_H
