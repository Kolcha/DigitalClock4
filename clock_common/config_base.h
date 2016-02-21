#ifndef CONFIGBASE_H
#define CONFIGBASE_H

#include <QObject>

#include <QVariant>
#include <QMap>

#include "clock_common_global.h"

class SettingsStorage;

class CLOCK_COMMON_EXPORT ConfigBase : public QObject
{
  Q_OBJECT
public:
  explicit ConfigBase(SettingsStorage* backend, QObject *parent = 0);

  SettingsStorage* GetBackend() const { return backend_; }

  const QVariant& GetValue(const int id) const;

signals:
  void reloaded();

public slots:
  void SetValue(const int id, const QVariant& value);

  void Load();
  void Save();

protected:
  virtual QString GetKey(const int id) const = 0;
  virtual QVariant GetDefaultValue(const int id) const = 0;

private:
  SettingsStorage* backend_;
  QMap<int, QVariant> current_values_;
};

#endif // CONFIGBASE_H
