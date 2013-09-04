#include <QDebug>
#include "alarm_thread.h"

AlarmThread::AlarmThread(QObject* parent)
  : QThread(parent) {
  setTerminationEnabled();
}

void AlarmThread::run() {
  qDebug("Alarm notification starts");
  QThread::sleep(2);
  qDebug("Alarm notification ends");
}
