#ifndef ALARM_THREAD_H
#define ALARM_THREAD_H

#include <QThread>

class AlarmThread : public QThread {
  Q_OBJECT
public:
  explicit AlarmThread(QObject* parent = 0);

protected:
  void run();
};

#endif // ALARM_THREAD_H
