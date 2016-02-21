#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <QObject>

#include <QSet>

#include "clock_common_global.h"

class SettingsStorage;

class CLOCK_COMMON_EXPORT ConfigManager : public QObject
{
  Q_OBJECT
public:
  explicit ConfigManager(QObject *parent = 0);

  void AddConfig(SettingsStorage* obj);
  void RemoveConfig(SettingsStorage* obj);

signals:

public slots:
  void Export(const QString& filename) const;
  void Import(const QString& filename) const;

private:
  QSet<SettingsStorage*> config_objs_;
};

#endif // CONFIG_MANAGER_H
