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

public slots:
  void AddPluginsDir(const QDir& dir);
  void DelPluginsDir(const QDir& dir);
  void ListAvailable();
  void LoadPlugins(const QStringList& files);
  void EnablePlugin(const QString& name, bool enable);

private:
  void LoadPlugin(const QString& file);
  void UnloadPlugin(const QString& file);

  QList<QDir> plugins_dirs_;
  QMap<QString, QString> available_;
  QMap<QString, QPluginLoader*> loaded_;
  TPluginData data_;
};

#endif // PLUGIN_MANAGER_H
