#ifndef SETTINGS_STORAGE_WRAPPER_H
#define SETTINGS_STORAGE_WRAPPER_H

#include <QObject>

#include <QString>
#include <QVariant>
#include <QSet>

#include "clock_common_global.h"

class SettingsStorage;

class CLOCK_COMMON_EXPORT SettingsStorageWrapper : public QObject
{
  Q_OBJECT
public:
  explicit SettingsStorageWrapper(SettingsStorage* backend, QObject* parent = 0);
  virtual ~SettingsStorageWrapper() {}

  /*!
   * Get storage backend.
   * @return pointer to SettingsStorage object
   */
  SettingsStorage* GetBackend() const { return backend_; }

signals:
  void accepted();
  void rejected();

public slots:
  void Accept();
  void Reject();

protected:
  void setValue(const QString& key, const QVariant& value);
  QVariant getValue(const QString& key, const QVariant& default_value = QVariant()) const;
  void remove(const QString& key);

private:
  SettingsStorage* backend_;
  QSet<QString> changed_keys_;
};

#endif // SETTINGS_STORAGE_WRAPPER_H
