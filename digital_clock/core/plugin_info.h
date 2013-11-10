#ifndef PLUGIN_INFO_H
#define PLUGIN_INFO_H

#include <QMap>

/*! plugin info fields */
enum FPluginInfo {
  PI_NAME,        /*!< plugin name */
  PI_TYPE,        /*!< plugin type (clock, settings, tray, widget) */
  PI_VERSION,     /*!< plugin version */
  PI_AUTHOR,      /*!< plugin author */
  PI_EMAIL,       /*!< author's e-mail */
  PI_COMMENT      /*!< plugin description */
};
/*! plugin info type */
typedef QMap<FPluginInfo, QString> TPluginInfo;

#endif // PLUGIN_INFO_H
