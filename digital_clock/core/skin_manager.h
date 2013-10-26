#ifndef SKIN_MANAGER_H
#define SKIN_MANAGER_H

#include <QDir>
#include <QString>
#include <QFont>
#include "iskin.h"

class SkinManager : public QObject {
  Q_OBJECT
public:
  explicit SkinManager(QObject* parent = 0);

signals:
  void SearchFinished(const QStringList& skins);
  void SkinLoaded(ISkin::ClockSkinPtr skin);
  void SkinInfoLoaded(const ISkin::TSkinInfo& info);

public slots:
  void AddSkinDir(const QDir& dir);
  void DelSkinDir(const QDir& dir);
  void ListSkins();
  void LoadSkin(const QString& skin_name);
  void SetFont(const QFont& font);

private:
  QList<QDir> skin_dirs_;
  QMap<QString, QDir> skins_;
  QFont font_;
};

#endif // SKIN_MANAGER_H
