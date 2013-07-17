#ifndef ICLOCK_PLUGIN_H
#define ICLOCK_PLUGIN_H

#include <QtPlugin>
#include "settings_keys.h"

/*! plugin info fields */
enum FPluginInfo {
  PI_NAME,        /*!< plugin name */
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
};


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

/*! Settings interface IID */
#define SETTINGS_PLUGIN_INTERFACE_IID   "nick-korotysh.digital-clock.settings-plugin"
Q_DECLARE_INTERFACE(ISettingsPlugin, SETTINGS_PLUGIN_INTERFACE_IID)

#endif // ICLOCK_PLUGIN_H
