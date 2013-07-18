#ifndef ICLOCK_PLUGIN_H
#define ICLOCK_PLUGIN_H

#include <QtPlugin>
#include "settings_keys.h"

/*! plugin info fields */
enum FPluginInfo {
  PI_NAME,        /*!< plugin name */
  PI_TYPE,        /*!< plugin type (base, settings, tray, widget) */
  PI_VERSION,     /*!< plugin version */
  PI_AUTHOR,      /*!< plugin author */
  PI_EMAIL,       /*!< author's e-mail */
  PI_COMMENT      /*!< plugin description */
};
/*! plugin info type */
typedef QMap<FPluginInfo, QString> TPluginInfo;


/*! Common plugins interface. */
class IClockPlugin : public QObject {
  Q_OBJECT

public:
  /*! Virtual destructor. */
  virtual ~IClockPlugin() {}
  /*! Start plugin activity. */
  virtual void Start() = 0;
  /*! Stop plugin activity. */
  virtual void Stop() = 0;
  /*! Call plugin settings dialog if it exists. */
  virtual void Configure() = 0;
  /*!
   * Get plugin info. @see TPluginInfo
   * @param info - variable to store plugin info
   */
  virtual void GetInfo(TPluginInfo* info) = 0;

public slots:
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
   * @param current_time - current time value
   * @note time format varies depend on system locale settings or plugins
   */
  virtual void TimeUpdateListener(const QString& current_time) = 0;
};

/*! Clock plugin interface IID */
#define CLOCK_PLUGIN_INTERFACE_IID   "nick-korotysh.digital-clock.clock-plugin"
Q_DECLARE_INTERFACE(IClockPlugin, CLOCK_PLUGIN_INTERFACE_IID)


/*!
 * @brief Settings plugin interface.
 *
 * Settings plugin can change any clock options. It must emit signal OptionChaged everytime
 * when it changes settings to notify clock. All changes will be applied immediately.
 */
class ISettingsPlugin : public IClockPlugin {
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
class QMenu;

/*!
 * @brief Tray plugin interface.
 *
 * Tray plugin can access to the clock tray icon in system tray. It can change clock
 * tray icon and modify tray menu (right click clock popup menu will modified too).
 */
class ITrayPlugin : public IClockPlugin {
  Q_OBJECT

public:
  /*! Virtual destructor. */
  virtual ~ITrayPlugin() {}
  /*!
   * Init plugin.
   * @param tray_icon - pointer to clock QSystemTrayIcon object
   * @param tray_menu - pointer to clock tray menu
   */
  virtual void Init(QSystemTrayIcon* tray_icon, QMenu* tray_menu) = 0;
};

/*! Tray plugin interface IID */
#define TRAY_PLUGIN_INTERFACE_IID   "nick-korotysh.digital-clock.tray-plugin"
Q_DECLARE_INTERFACE(ITrayPlugin, TRAY_PLUGIN_INTERFACE_IID)


/*!
 * @brief Widget plugin interface.
 *
 * Widget plugin can access and manipulate with main clock window. It can change
 * window position or add some widgets to this window for example.
 */
class IWidgetPlugin : public IClockPlugin {
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
