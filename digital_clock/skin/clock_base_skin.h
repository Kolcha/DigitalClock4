#ifndef CLOCK_BASE_SKIN_H
#define CLOCK_BASE_SKIN_H

#include <QString>
#include <QSharedPointer>

class ClockBaseSkin {
public:
  ClockBaseSkin();
  virtual ~ClockBaseSkin();

  void SetSeparators(const QString& seps);

protected:
  virtual void ProcSeparators() = 0;

  QString seps_;
};

typedef QSharedPointer<ClockBaseSkin> ClockSkinPtr;

#endif // CLOCK_BASE_SKIN_H
