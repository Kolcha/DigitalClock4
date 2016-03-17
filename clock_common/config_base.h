#ifndef CONFIG_BASE_H
#define CONFIG_BASE_H

#include <QObject>

#include <QVariant>
#include <QMap>

#include "clock_common_global.h"

class SettingsStorage;

/*!
 * @brief Base class for configs.
 *
 * Provides base config logic: load/save values from/to permanent storage.
 */
class CLOCK_COMMON_EXPORT ConfigBase : public QObject
{
  Q_OBJECT
public:
  /*!
   * Constructor
   * @param backend - settings storage backend
   * @param parent - parent object
   */
  explicit ConfigBase(SettingsStorage* backend, QObject *parent = 0);
  /*! Destructor */
  virtual ~ConfigBase() {}

  /*!
   * Get storage backend.
   * @return pointer to SettingsStorage object
   */
  SettingsStorage* GetBackend() const { return backend_; }

  /*!
   * @brief Get option value.
   *
   * Tries to get value from backend, and if have no success returns option default value.
   * @param id - option id
   * @return option value
   * @see ConfigBase::GetDefaultValue(), SettingsStorage::GetValue()
   */
  const QVariant& GetValue(const int id) const;

signals:
  /*! Emitted after loading from permanent storage */
  void reloaded();
  /*! Emitted after saving to permanent storage */
  void saved();

public slots:
  /*!
   * Change option value on runtime.
   * @param id - option id
   * @param value - option value
   * @note permanent storage will not modified
   */
  void SetValue(const int id, const QVariant& value);

  /*! Load current settings from permanent storage */
  void Load();
  /*! Save current settings to permanent storage */
  void Save();

protected:
  /*!
   * Get string key for given option @a id
   * @note each key must be unique
   */
  virtual QString GetKey(const int id) const = 0;
  /*!
   * Get default value for given option @a id
   * @note every option must have valid default value
   */
  virtual QVariant GetDefaultValue(const int id) const = 0;

private:
  /// settings storage backend
  SettingsStorage* backend_;
  /// map with current (runtime) values
  QMap<int, QVariant> current_values_;
};

#endif // CONFIG_BASE_H
