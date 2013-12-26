#ifndef PLUGIN_INFO_H
#define PLUGIN_INFO_H

#include <QMap>
#include "plugin_gui_info.h"

/*! plugin metadata fields */
enum FPluginInfo {
  PI_NAME,        /*!< plugin internal name */
  PI_VERSION,     /*!< plugin version */
  PI_AUTHOR,      /*!< plugin author */
  PI_EMAIL,       /*!< author's e-mail */
};

/*! plugin info type */
struct TPluginInfo {
  TPluginGUIInfo gui_info;              /*!< translatable plugin info */
  QMap<FPluginInfo, QString> metadata;  /*!< plugin metadata and other info */
};

#endif // PLUGIN_INFO_H
