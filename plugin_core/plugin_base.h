/*
    Digital Clock: clock common library
    Copyright (C) 2013-2017  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PLUGIN_BASE_H
#define PLUGIN_BASE_H

#include "plugin_core_global.h"
#include <QObject>
#include <QPixmap>
#include <QString>

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
  QPixmap icon;           /*!< plugin icon (logo) */
};

class QTranslator;
class SettingsStorage;
class PluginSettings;

/*!
 * @brief Base class for plugins.
 *
 * This class provides some common logic for all plugins and it inherited by all plugin interfaces.
 * Do not use it directly, use only interfaces.
 */
class PLUGIN_CORE_SHARED_EXPORT PluginBase : public QObject
{
  Q_OBJECT

public:
  /*! Constructor. */
  PluginBase();
  /*! Destructor. */
  virtual ~PluginBase();
  /*!
   * Get translatable plugin info.
   * @return structure with plugin info
   */
  const TPluginGUIInfo& GetInfo() const;
  /*!
   * Initializes plugin configuration storage.
   * @param backend - config storage backend
   * @param name - current plugin's name
   */
  virtual void InitSettings(SettingsStorage* backend, const QString& name);

protected:
  /*!
   * Autodetect system UI language and load translation.
   * @param prefix - language file path prefix (example: ":/lang/example_")
   */
  void InitTranslator(const QLatin1String& prefix);
  /*!
   * Load plugin icon and init need filed in plugin info structure with it.
   * @param file_path - path to icon file (svg file from plugin resource, for example)
   * @see TPluginGUIInfo
   */
  void InitIcon(const QString& file_path);
  /// Structure with translatable plugin info. This must be filled in child constructor.
  TPluginGUIInfo info_;
  /// PluginSettings object, will be available only if plugin is configurable
  PluginSettings* settings_;

private:
  QTranslator* translator_;
};

#endif // PLUGIN_BASE_H
