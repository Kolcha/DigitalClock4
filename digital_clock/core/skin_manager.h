#ifndef DIGITAL_CLOCK_CORE_SKIN_MANAGER_H
#define DIGITAL_CLOCK_CORE_SKIN_MANAGER_H

#include <QString>
#include <QFont>
#include "iskin.h"
#include "../skin/clock_base_skin.h"
#include "manager_base.h"

namespace digital_clock {
namespace core {

class SkinManager : public ManagerBase {
  Q_OBJECT

public:
  explicit SkinManager(QObject* parent = 0);
  ClockSkinPtr CurrentSkin() const;

signals:
  void SearchFinished(const QStringList& skins);
  void SkinLoaded(skin_draw::ISkin::SkinPtr skin);
  void SkinInfoLoaded(const BaseSkin::TSkinInfo& info);
  void ErrorMessage(const QString& msg);

public slots:
  void ListSkins();
  void LoadSkin(const QString& skin_name);
  void SetFont(const QFont& font);
  void SetSeparators(const QString& seps);
  void SetFallbackSkin(const QString& skin_name);

private:
  QMap<QString, QDir> skins_;
  QFont font_;
  QString seps_;
  ClockSkinPtr current_skin_;
  QString fallback_skin_;
};

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_SKIN_MANAGER_H
