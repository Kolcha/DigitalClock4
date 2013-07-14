#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <QObject>

class PluginManager : public QObject {
  Q_OBJECT
public:
  explicit PluginManager(QObject *parent = 0);

signals:

public slots:

};

#endif // PLUGIN_MANAGER_H
