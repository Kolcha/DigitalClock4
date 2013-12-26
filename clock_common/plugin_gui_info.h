#ifndef PLUGIN_GUI_INFO_H
#define PLUGIN_GUI_INFO_H

#include <QImage>

/*!
 * @brief Translatable plugin info structure.
 *
 * Fields of this structure contain different plugin information that can be translated
 * (inside plugin) and displayed anywhere in any dialog.
 * @see IClockPlugin::GetInfo()
 */
struct TPluginGUIInfo {
  QString display_name;   /*!< plugin display name (user friendly) */
  QString description;    /*!< plugin description */
  QImage icon;            /*!< plugin icon (logo) */
};

#endif // PLUGIN_GUI_INFO_H
