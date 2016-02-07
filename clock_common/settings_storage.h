#ifndef SETTINGS_STORAGE_H
#define SETTINGS_STORAGE_H

#include <QObject>

#include <QSettings>

#include "clock_common_global.h"

class CLOCK_COMMON_EXPORT SettingsStorage : public QObject
{
  Q_OBJECT
public:
  explicit SettingsStorage(QObject *parent = 0);

  const QVariant& GetValue(const QString& key, const QVariant& default_value = QVariant());
  void SetValue(const QString& key, const QVariant& value);

signals:
  void reloaded();

public slots:
  void Load();
  void Save();

  void Reset();

  void Export(const QString& filename);
  void Import(const QString& filename);

private:
  QSettings settings_;
  QSettings::SettingsMap current_values_;
};

#endif // SETTINGS_STORAGE_H
