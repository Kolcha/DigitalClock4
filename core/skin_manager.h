#ifndef SKIN_MANAGER_H
#define SKIN_MANAGER_H

#include <QMap>
#include <QString>
#include <QStringList>

class SkinManager : public QObject {
  Q_OBJECT
public:
  explicit SkinManager(QObject *parent = 0);

signals:
  void SearchFinished(const QStringList& skins);
  void SkinFound(const QString& skin_root);

public slots:
  void AddSkinDir(const QString& dir);
  void DelSkinDir(const QString& dir);
  void ListSkins();
  void FindSkin(const QString& skin_name);

private:
  QStringList skin_dirs_;
  QMap<QString, QString> skins_;
};

#endif // SKIN_MANAGER_H
