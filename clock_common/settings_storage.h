/*
    Digital Clock: clock common library
    Copyright (C) 2016-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef SETTINGS_STORAGE_H
#define SETTINGS_STORAGE_H

#include <QObject>

#include <QStringList>
#include <QSettings>
#include <QSet>

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
  explicit SettingsStorage(QObject* parent = 0);

  /*!
   * Change option value only in runtime storage.
   * @param key - key to change
   * @param value - option value
   */
  void SetValue(const QString& key, const QVariant& value);
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
  QVariant GetValue(const QString& key, const QVariant& default_value = QVariant());

  /*!
   * @brief Remove @a key and all its child elements.
   *
   * @a key will removed from runtime storage, actual remove will happen only after Commit().
   * Until remove in not committed, removed @a key can be restored with Revert().
   * @see Commit(), Revert(), Accept(), Reject()
   */
  void Remove(const QString& key);

  /*!
   * @brief Find and list all one-level child elements for given @a key.
   * @return list of child elements (NOT including parent path)
   */
  QStringList ListChildren(const QString& key);

  /*!
   * @brief Write all @a key changes to permanent storage.
   * @see Revert(), Accept(), Reject()
   */
  void Commit(const QString& key);
  /*!
   * @brief Restore @a key value from permanent storage.
   * @see Commit(), Accept(), Reject()
   */
  void Revert(const QString& key);

  /*!
   * Remove option with given @a key from runtime storage.
   * @note value in permanent storage is not changed
   */
  void Forget(const QString& key);

signals:
  /*! Notifies about whole runtime storage change */
  void reloaded();

public slots:
  /*! Export current settings to file */
  void Export(const QString& filename);
  /*! Import current settings from file */
  void Import(const QString& filename);

  /*! Save current settings in runtime storage to permanent storage */
  void Accept();
  /*! Load settings from permanent storage to runtime storage */
  void Reject();

  /*! Reset current settings to their default values */
  void Reset();  // TODO: implement

private:
  /*!
   * @brief Checks is @a key deleted.
   * @return true if @a key was removed, otherwise returns false
   * @see Remove()
   */
  bool isDeleted(const QString& key) const;
  /*!
   * @brief Finds all child elements for @a key in given map @a m.
   * @return list of child elements
   */
  static QStringList findKeyChildren(const QString& key, const QSettings::SettingsMap& m);

  /// permanent storage (QSettings backend)
  QSettings storage_;
  /// temporary container for imported values
  /// @see Import(), Export()
  QSettings::SettingsMap imported_;
  /// runtime storage
  QSettings::SettingsMap current_;
  /// runtime list of removed keys
  QSet<QString> removed_keys_;
};

#endif // SETTINGS_STORAGE_H
