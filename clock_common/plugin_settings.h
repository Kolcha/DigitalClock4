#ifndef PLUGIN_SETTINGS_H
#define PLUGIN_SETTINGS_H

#include <QObject>

#include <QSettings>

#include "clock_common_global.h"


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
class CLOCK_COMMON_EXPORT PluginSettings : public QObject {
  Q_OBJECT

public:
  /*!
   * Constructor.
   * @param backend - storage backend
   * @param parent - parent object
   */
  PluginSettings(SettingsStorage* backend, QObject* parent = 0);

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
  const QVariant& GetOption(const QString& key) const;

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

private:
  SettingsStorage* backend_;
  QSettings::SettingsMap default_map_;
  bool track_changes_;
};

#endif // PLUGIN_SETTINGS_H
