#ifndef CLOCK_BASE_SKIN_H
#define CLOCK_BASE_SKIN_H

#include <QString>
#include <QSharedPointer>

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

  ClockBaseSkin();
  virtual ~ClockBaseSkin();

  const TSkinInfo& GetInfo() const;
  void SetSeparators(const QString& seps);

protected:
  virtual void ProcSeparators() = 0;

  TSkinInfo info_;
  QString seps_;
};

typedef QSharedPointer<ClockBaseSkin> ClockSkinPtr;

#endif // CLOCK_BASE_SKIN_H
