#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <QMap>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QPluginLoader>
#include "iclock_plugin.h"

class PluginManager : public QObject {
  Q_OBJECT
public:
  explicit PluginManager(QObject *parent = 0);
  void SetInitData(const TPluginData& data);

signals:
  void SearchFinished(const QStringList& skins);
  void InfoGot(const TPluginInfo& info);

public slots:
  void AddPluginsDir(const QDir& dir);
  void DelPluginsDir(const QDir& dir);
  void ListAvailable();
  void LoadPlugins(const QStringList& names);
  void EnablePlugin(const QString& name, bool enable);
  void GetPluginInfo(const QString& name);

private:
  void LoadPlugin(const QString& name);
  void UnloadPlugin(const QString& name);

  QList<QDir> plugins_dirs_;
  QMap<QString, QString> available_;
  QMap<QString, QPluginLoader*> loaded_;
  TPluginData data_;
};

#endif // PLUGIN_MANAGER_H
