#ifndef DIGITAL_CLOCK_CORE_CLOCK_BASE_SKIN_H
#define DIGITAL_CLOCK_CORE_CLOCK_BASE_SKIN_H

#include <QMap>
#include <QString>
#include <QSharedPointer>

namespace digital_clock {
namespace core {

class ClockBaseSkin {
public:
  enum FSkinInfo {
    SI_NAME,        // skin name
    SI_VERSION,     // skin version
    SI_AUTHOR,      // skin author
    SI_EMAIL,       // author's e-mail
    SI_COMMENT      // skin description
  };
  typedef QMap<FSkinInfo, QString> TSkinInfo;

  virtual ~ClockBaseSkin() {}

  const TSkinInfo& GetInfo() const;
  void SetSeparators(const QString& seps);

protected:
  virtual void ProcSeparators() = 0;

  TSkinInfo info_;
  QString seps_;
};

typedef QSharedPointer<ClockBaseSkin> ClockSkinPtr;

} // namespace core
} // namespace digital_clock

#endif // DIGITAL_CLOCK_CORE_CLOCK_BASE_SKIN_H
