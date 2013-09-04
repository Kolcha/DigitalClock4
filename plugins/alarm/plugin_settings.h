#ifndef PLUGIN_SETTINGS_H
#define PLUGIN_SETTINGS_H

#include <QSettings>

/*!
 * @brief The PluginSettings class.
 *
 * PluginSettings class provides logic to Save/Load clock plugin settings.
 * This class is based on QSettings class.
 */
class PluginSettings : public QObject {
  Q_OBJECT

public:
  /*!
   * Constructor.
   * @param org_name - organization/developer name (same as for QSettings)
   * @param app_name - application name (same as for QSettings)
   * @param parent - parent object
   */
  PluginSettings(const QString& org_name, const QString& app_name, QObject* parent = 0)
    : QObject(parent), settings_(org_name, app_name), track_changes_(false) {}

  /*!
   * Init all options with default values.
   * @param values - map<QString, QVariant> with option names and their values
   */
  void SetDefaultValues(const QSettings::SettingsMap& values) {
    settings_map_ = values;
  }
  /*!
   * Get option value.
   * @param key - option name
   * @return option value as QVariant object
   */
  const QVariant& GetOption(const QString& key) const {
    return settings_map_.find(key).value();
  }

public slots:
  /*!
   * Load settings.
   */
  void Load() {
    for (auto iter = settings_map_.begin(); iter != settings_map_.end(); ++iter) {
      settings_map_[iter.key()] = settings_.value(iter.key(), iter.value());
      if (track_changes_) emit OptionChanged(iter.key(), iter.value());
    }
  }
  /*!
   * Save settings.
   */
  void Save() {
    for (auto iter = settings_map_.begin(); iter != settings_map_.end(); ++iter) {
      settings_.setValue(iter.key(), iter.value());
    }
  }

  /*!
   * Set option value.
   * @param key - option name
   * @param value - option value
   */
  void SetOption(const QString& key, const QVariant& value) {
    settings_map_[key] = value;
    if (track_changes_) emit OptionChanged(key, value);
  }

  /*!
   * Set 'track changes' flag. If it is set then signal OptionChanged() will emitted
   * when any option is changed.
   * @param is_track - true to enable tracking, false to disable
   */
  void TrackChanges(bool is_track) {
    track_changes_ = is_track;
  }

signals:
  /*!
   * Signal to notify that option have changed.
   * @param key - option name which was changed
   * @param value - new option value as QVariant
   */
  void OptionChanged(const QString& key, const QVariant& value);

private:
  QSettings settings_;
  QSettings::SettingsMap settings_map_;
  bool track_changes_;
};

#endif // PLUGIN_SETTINGS_H
