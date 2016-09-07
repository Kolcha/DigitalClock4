#ifndef CONFIG_BASE_H
#define CONFIG_BASE_H

#include "settings_storage_wrapper.h"

#include "clock_common_global.h"

/*!
 * @brief Base class for configs.
 *
 * Provides base config logic: load/save values from/to permanent storage.
 */
class CLOCK_COMMON_EXPORT ConfigBase : public SettingsStorageWrapper
{
  Q_OBJECT
public:
  /*!
   * Constructor
   * @param backend - settings storage backend
   * @param parent - parent object
   */
  explicit ConfigBase(SettingsStorage* backend, QObject* parent = 0);
  /*! Destructor */
  virtual ~ConfigBase() {}

  /*!
   * @brief Get option value.
   *
   * Tries to get value from backend, and if have no success returns option default value.
   * @param id - option id
   * @return option value
   * @see ConfigBase::GetDefaultValue(), SettingsStorage::GetValue()
   */
  QVariant GetValue(const int id) const;

public slots:
  /*!
   * Change option value on runtime.
   * @param id - option id
   * @param value - option value
   * @note permanent storage will not modified
   */
  void SetValue(const int id, const QVariant& value);

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
};

#endif // CONFIG_BASE_H
