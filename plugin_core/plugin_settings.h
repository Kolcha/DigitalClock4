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

#ifndef PLUGIN_SETTINGS_H
#define PLUGIN_SETTINGS_H

#include <QObject>

#include <QSettings>

#include "plugin_core_global.h"


class SettingsStorage;

/*!
 * @brief Plugin settings save/load class.
 *
 * PluginSettings class provides logic to Save/Load any clock plugin settings.
 * This class is based on QSettings class.
 *
 * This class can notify about value change emitting signal PluginSettings::OptionChanged().
 * This feature is disabled by default, to enable it see PluginSettings::TrackChanges().
 */
class PLUGIN_CORE_SHARED_EXPORT PluginSettings : public QObject
{
  Q_OBJECT

public:
  /*!
   * Constructor.
   * @param backend - storage backend
   * @param name - plugin's name
   * @param parent - parent object
   */
  PluginSettings(SettingsStorage* backend, const QString& name, QObject* parent = nullptr);
  /*! Destructor. */
  ~PluginSettings();

  /*!
   * Init all options with default values.
   * @param values - map<QString, QVariant> with option names and their values
   * @note This method must be called before Load() to init internal data.
   */
  void SetDefaultValues(const QSettings::SettingsMap& values);
  /*!
   * Get option value.
   * @param key - option name
   * @return option value as QVariant object
   */
  QVariant GetOption(const QString& key) const;

public slots:
  /*! Load settings. */
  void Load();
  /*! Save settings. */
  void Save();
  /*!
   * Set option value.
   * @param key - option name
   * @param value - option value
   */
  void SetOption(const QString& key, const QVariant& value);
  /*!
   * Set 'track changes' flag. If it is set then signal OptionChanged() will emitted
   * when any option is changed.
   * @param is_track - true to enable tracking, false to disable
   * @note When 'track changes' is enabled before Load() call, it will emit signal OptionChanged()
   * for every loaded value.
   */
  void TrackChanges(bool is_track);

signals:
  /*!
   * Signal to notify that option have changed.
   * @param key - option name which was changed
   * @param value - new option value as QVariant
   */
  void OptionChanged(const QString& key, const QVariant& value);

private slots:
  void Reload();

private:
  /*!
   * Converts plugin-specific config @a key to unique key (path) in global settings storage.
   * @return unique path in settings storage
   */
  QString WrapKey(const QString& key) const;

private:
  SettingsStorage* backend_;
  QSettings::SettingsMap default_map_;
  QSettings::SettingsMap current_map_;
  bool track_changes_;
  const QString prefix_mask_;     // prefix must not have '/' at the end!
  const QString plugin_name_;
};

#endif // PLUGIN_SETTINGS_H
