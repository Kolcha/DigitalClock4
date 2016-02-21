#ifndef SETTINGS_STORAGE_H
#define SETTINGS_STORAGE_H

#include <QObject>

#include <QSettings>

#include "clock_common_global.h"

class ConfigManager;

class CLOCK_COMMON_EXPORT SettingsStorage : public QObject
{
  Q_OBJECT
public:
  explicit SettingsStorage(ConfigManager* manager, QObject *parent = 0);
  virtual ~SettingsStorage();

  const QVariant& GetValue(const QString& key, const QVariant& default_value = QVariant());
  void SetValue(const QString& key, const QVariant& value);

  void Export(QSettings::SettingsMap* m) const;
  void Import(const QSettings::SettingsMap& m);

signals:
  void reloaded();

public slots:
  void Load();
  void Save();

  void Reset();

private:
  ConfigManager* manager_;
  QSettings settings_;
  QSettings::SettingsMap current_values_;
  QSettings::SettingsMap default_values_;
};

#endif // SETTINGS_STORAGE_H
