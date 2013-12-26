/*!
 * @file iclock_plugin.h
 *
 * @brief This file contains all plugins interfaces.
 *
 * Digital Clock has next plugin types:
 * @li clock
 * @li settings
 * @li tray
 * @li widget
 *
 * Clock plugin is a base plugin interface for all other plugin types. This is only for very
 * simple plugins, that doesn't need in access to clock settings, tray icon or other things.
 * See IClockPlugin interface for details.
 *
 * Settings plugin can change any clock setting. You must be sure that your plugin set correct
 * setting values, there is no any check inside clock, be carefull. To get all possible settings
 * see settings_keys.h. See ISettingsPlugin interface for details.
 *
 * Tray plugin can access to clock's system tray icon and do with it anything: show/hide, change
 * icon/tooltip, display messages from tray. It has access to tray menu too. You can add/remove
 * any tray menu items from plugin. See ITrayPlugin interface for details.
 *
 * Widget plugin has access to clock's main window (as QWidget pointer). This plugin type planned
 * to modify clock's view, add date or weather string for example. Main window layout will be
 * described later.
 * You can use this plugin type to create simple configurable plugin without any clock window
 * modifications. See IWidgetPlugin interface for details.
 */

#ifndef ICLOCK_PLUGIN_H
#define ICLOCK_PLUGIN_H

#include <QtPlugin>
#include "plugin_gui_info.h"
#include "settings_keys.h"

/*!
 * @brief Clock plugin interface.
 *
 * Common interface for all plugins.
 */
class CLOCK_COMMON_EXPORT IClockPlugin : public QObject {
  Q_OBJECT

public:
  /*! Virtual destructor. */
  virtual ~IClockPlugin() {}
  /*!
   * Get translatable plugin info.
   * @return structure with plugin info
   */
  const TPluginGUIInfo& GetInfo() const { return info_; }

public slots:
  /*! Start plugin activity. */
  virtual void Start() = 0;
  /*! Stop plugin activity. */
  virtual void Stop() = 0;
  /*! Call plugin settings dialog if it exists. */
  virtual void Configure() = 0;
  /*!
   * @brief Clock settings changes listener.
   *
   * This slot will be called when any clock option will changed.
   * @param option - option which was changed. @see Options
   * @param new_value - new option value as QVariant
   */
  virtual void SettingsListener(Options option, const QVariant& new_value) = 0;
  /*!
   * @brief Current time update listener.
   *
   * This slot will be called every 0.5 sec when clock updates current time.
   * It can be used as timer. Clock provides current time value as QString.
   */
  virtual void TimeUpdateListener() = 0;

protected:
  /// Structure with translatable plugin info. This must be filled in constructor.
  TPluginGUIInfo info_;
};

/*! Clock plugin interface IID */
#define CLOCK_PLUGIN_INTERFACE_IID   "nick-korotysh.digital-clock.clock-plugin"
Q_DECLARE_INTERFACE(IClockPlugin, CLOCK_PLUGIN_INTERFACE_IID)


/*!
 * @brief Settings plugin interface.
 *
 * Settings plugin can change any clock options. It must emit signal OptionChaged everytime
 * when it changes settings to notify clock. All changes will be applied immediately.
 * @note You must be sure that your plugin set correct option values, there is no any check
 * inside clock, be carefull.
 * @see settings_keys.h.
 */
class CLOCK_COMMON_EXPORT ISettingsPlugin : public IClockPlugin {
  Q_OBJECT

public:
  /*! Virtual destructor. */
  virtual ~ISettingsPlugin() {}
  /*!
   * Init plugin.
   * @param current_settings - map with all current clock settings
   */
  virtual void Init(const QMap<Options, QVariant>& current_settings) = 0;

signals:
  /*!
   * Signal to notify clock that option have changed.
   * @param option - option which was changed. @see Options
   * @param value - new option value as QVariant
   */
  void OptionChanged(Options option, const QVariant& value);
};

/*! Settings plugin interface IID */
#define SETTINGS_PLUGIN_INTERFACE_IID   "nick-korotysh.digital-clock.settings-plugin"
Q_DECLARE_INTERFACE(ISettingsPlugin, SETTINGS_PLUGIN_INTERFACE_IID)

class QSystemTrayIcon;

/*!
 * @brief Tray plugin interface.
 *
 * Tray plugin can access to the clock tray icon in system tray. It can change clock
 * tray icon and modify tray menu (right click clock popup menu will modified too).
 */
class CLOCK_COMMON_EXPORT ITrayPlugin : public IClockPlugin {
  Q_OBJECT

public:
  /*! Virtual destructor. */
  virtual ~ITrayPlugin() {}
  /*!
   * Init plugin.
   * @param tray_icon - pointer to clock QSystemTrayIcon object
   */
  virtual void Init(QSystemTrayIcon* tray_icon) = 0;
};

/*! Tray plugin interface IID */
#define TRAY_PLUGIN_INTERFACE_IID   "nick-korotysh.digital-clock.tray-plugin"
Q_DECLARE_INTERFACE(ITrayPlugin, TRAY_PLUGIN_INTERFACE_IID)


/*!
 * @brief Widget plugin interface.
 *
 * Widget plugin can access and manipulate with main clock window. It can change
 * window position or add/remove some widgets to this window.
 * For example, it can add new widget to date or day of week.
 */
class CLOCK_COMMON_EXPORT IWidgetPlugin : public IClockPlugin {
  Q_OBJECT

public:
  /*! Virtual destructor. */
  virtual ~IWidgetPlugin() {}
  /*!
   * Init plugin.
   * @param main_wnd - pointer to clock main window
   */
  virtual void Init(QWidget* main_wnd) = 0;
};

/*! Widget plugin interface IID */
#define WIDGET_PLUGIN_INTERFACE_IID   "nick-korotysh.digital-clock.widget-plugin"
Q_DECLARE_INTERFACE(IWidgetPlugin, WIDGET_PLUGIN_INTERFACE_IID)

#endif // ICLOCK_PLUGIN_H
