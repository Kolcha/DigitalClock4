#ifndef SETTINGS_STORAGE_H
#define SETTINGS_STORAGE_H

#include <QObject>

#include <QSettings>
#include <QSet>
#include <QStringList>

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

  /*!
   * Permanently remove given @a key and its child elements.
   * @note data will removed actually only on Save()
   */
  void Delete(const QString& key);

  /*!
   * Get all child elements for specified @a key.
   * @return list of child elements
   */
  QStringList ListChildren(const QString& key);

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
  /*!
   * Checks is given @a key deleted.
   * @return true if key was deleted
   */
  bool isDeleted(const QString& key);
  /*!
   * Helper function to find all child keys for given @a key in given map @a m.
   * @return list of child keys if they were found
   */
  static QStringList findKeyChildren(const QString& key, const QSettings::SettingsMap& m);

  /// permanent storage (QSettings backend)
  QSettings settings_;
  /// runtime storage
  QSettings::SettingsMap current_values_;
  /// default values storage
  QSettings::SettingsMap default_values_;
  /// removed keys storage, will be cleared on Load() or Save()
  QSet<QString> removed_keys_;
};

#endif // SETTINGS_STORAGE_H
