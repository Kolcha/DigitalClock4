#ifndef DIGITAL_CLOCK_CORE_BASE_SKIN_H
#define DIGITAL_CLOCK_CORE_BASE_SKIN_H

#include <QMap>
#include <QString>
#include <QSharedPointer>

namespace digital_clock {
namespace core {

class BaseSkin {
public:
  enum FSkinInfo {
    SI_NAME,        // skin name
    SI_VERSION,     // skin version
    SI_AUTHOR,      // skin author
    SI_EMAIL,       // author's e-mail
    SI_COMMENT      // skin description
  };
  typedef QMap<FSkinInfo, QString> TSkinInfo;

  virtual ~BaseSkin() {}

  const TSkinInfo& GetInfo() const;
  void SetSeparators(const QString& seps);

protected:
  virtual void ProcSeparators() = 0;

  TSkinInfo info_;
  QString seps_;
};

typedef QSharedPointer<BaseSkin> ClockSkinPtr;

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_BASE_SKIN_H
