#ifndef SETTINGS_STORAGE_H
#define SETTINGS_STORAGE_H

#include <QObject>

#include <QSettings>

#include "clock_common_global.h"

/*!
 * @brief Storage backend for settings.
 *
 * This class stores only currently used values (not all existing values).
 * It reads required value from permanent storage at first time when it requested.
 * Next times value will be got from runtime storage.
 *
 * When plugin has unloaded, it unregister own option key from this backend.
 *
 * This class implements logic to export/import current settings.
 * @note Only currently used value will be exported.
 * @note Even runtime (not applied or commited) values can be exported.
 */
class CLOCK_COMMON_EXPORT SettingsStorage : public QObject
{
  Q_OBJECT
public:
  /*! Constructor */
  explicit SettingsStorage(QObject *parent = 0);

  /*!
   * @brief Get option value.
   *
   * First of it tries to get value from runtime storage, if have no success it tries
   * read permanent storage, and if have no success again it returns given @a default_value.
   * @param key - option key
   * @param default_value - default value to return if nothing was found
   * @return option value
   * @note given @a default_value will be saved for internal usage
   */
  const QVariant& GetValue(const QString& key, const QVariant& default_value = QVariant());
  /*!
   * Change option value only in runtime storage.
   * @param key - key to change
   * @param value - option value
   */
  void SetValue(const QString& key, const QVariant& value);
  /*!
   * Writes (commits) option from runtime storage to permanent storage.
   * @param key - option key
   * @param value - option value
   */
  void CommitValue(const QString& key, const QVariant& value);

  /*!
   * Remove option with given @a key from runtime storage.
   * @note value in permanent storage is not changed
   */
  void Remove(const QString& key);

signals:
  /*! Notifies about whole runtime storage change */
  void reloaded();

public slots:
  /*! Load settings from permanent storage to runtime storage */
  void Load();
  /*! Save current settings in runtime storage to permanent storage */
  void Save();

  /*! Reset current settings to their default values */
  void Reset();

  /*! Export current settings to file */
  void Export(const QString& filename);
  /*! Import current settings from file */
  void Import(const QString& filename);

private:
  /// permanent storage (QSettings backend)
  QSettings settings_;
  /// runtime storage
  QSettings::SettingsMap current_values_;
  /// default values storage
  QSettings::SettingsMap default_values_;
};

#endif // SETTINGS_STORAGE_H
