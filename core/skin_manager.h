#ifndef SKIN_MANAGER_H
#define SKIN_MANAGER_H

#include <QDir>
#include <QMap>
#include <QString>

class SkinManager : public QObject {
  Q_OBJECT
public:
  explicit SkinManager(QObject *parent = 0);

signals:
  void SearchFinished(const QStringList& skins);
  void SkinFound(const QDir& skin_root);

public slots:
  void AddSkinDir(const QDir& dir);
  void DelSkinDir(const QDir& dir);
  void ListSkins();
  void FindSkin(const QString& skin_name);

private:
  QList<QDir> skin_dirs_;
  QMap<QString, QDir> skins_;
};

#endif // SKIN_MANAGER_H
