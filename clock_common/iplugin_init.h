/*!
 * @file iplugin_init.h
 *
 * @brief This file contains interfaces for plugin initialization.
 *
 * There are interfaces only for plugin initialization. They must be used with IClockPlugin or
 * derived class. Some interfaces may be used for one plugin to create 'hybrid' plugin ('tray'
 * and 'widget' for example). Any interface may be used with each other interface.
 * @see iclock_plugin.h
 */

#ifndef IPLUGIN_INIT_H
#define IPLUGIN_INIT_H

#include <QtPlugin>
#include "settings_keys.h"

/*!
 * @brief Settings plugin initializer interface.
 *
 * Interface for 'settings' plugin initialization. This interface allows plugin to get current
 * clock settings.
 * Settings initializer is called first of all initializers.
 */
class ISettingsPluginInit {
public:
  /*!
   * Virtual destructor.
   */
  virtual ~ISettingsPluginInit() {}
  /*!
   * Init plugin.
   * @param current_settings - map with all current clock settings
   */
  virtual void Init(const QMap<Option, QVariant>& current_settings) = 0;
};

/*! Settings plugin init interface IID */
#define SETTINGS_PLUGIN_INIT_INTERFACE_IID   "nick-korotysh.digital-clock.settings-plugin-init"
Q_DECLARE_INTERFACE(ISettingsPluginInit, SETTINGS_PLUGIN_INIT_INTERFACE_IID)

class QSystemTrayIcon;

/*!
 * @brief Tray plugin initializer interface.
 *
 * Interface for 'tray' plugin initialization. This interface allows plugin to get access to
 * clock's tray icon.
 */
class ITrayPluginInit {
public:
  /*!
   * Virtual destructor.
   */
  virtual ~ITrayPluginInit() {}
  /*!
   * Init plugin.
   * @param tray_icon - pointer to clock QSystemTrayIcon object
   */
  virtual void Init(QSystemTrayIcon* tray_icon) = 0;
};

/*! Tray plugin init interface IID */
#define TRAY_PLUGIN_INIT_INTERFACE_IID   "nick-korotysh.digital-clock.tray-plugin-init"
Q_DECLARE_INTERFACE(ITrayPluginInit, TRAY_PLUGIN_INIT_INTERFACE_IID)


/*!
 * @brief Widget plugin initializer interface.
 *
 * Interface for 'widget' plugin initialization. This interface allows plugin to get access to
 * clock's main window.
 * Main window has grid layout (QGridLayout). Clock widget is located in cell (0,0).
 */
class IWidgetPluginInit {
public:
  /*!
   * Virtual destructor.
   */
  virtual ~IWidgetPluginInit() {}
  /*!
   * Init plugin.
   * @param main_wnd - pointer to clock main window
   */
  virtual void Init(QWidget* main_wnd) = 0;
};

/*! Widget plugin init interface IID */
#define WIDGET_PLUGIN_INIT_INTERFACE_IID   "nick-korotysh.digital-clock.widget-plugin-init"
Q_DECLARE_INTERFACE(IWidgetPluginInit, WIDGET_PLUGIN_INIT_INTERFACE_IID)

class ISkinDrawer;

/*!
 * @brief Drawer plugin initializer interface.
 *
 * Interface for 'drawer' plugin initialization. This interface allows plugin to get access to
 * clock's skin drawing engine and draw own information using it.
 * @see ISkinDrawer
 */
class IDrawerPluginInit {
public:
  /*!
   * Virtual destructor.
   */
  virtual ~IDrawerPluginInit() {}
  /*!
   * Init plugin.
   * @param drawer - configured drawer instance
   */
  virtual void Init(ISkinDrawer* drawer) = 0;
};

/*! Drawer plugin init interface IID */
#define DRAWER_PLUGIN_INIT_INTERFACE_IID   "nick-korotysh.digital-clock.drawer-plugin-init"
Q_DECLARE_INTERFACE(IDrawerPluginInit, DRAWER_PLUGIN_INIT_INTERFACE_IID)

#endif // IPLUGIN_INIT_H
