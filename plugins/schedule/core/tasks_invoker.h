#ifndef SCHEDULE_TASKS_INVOKER_H
#define SCHEDULE_TASKS_INVOKER_H

#include <QObject>

#include <QDateTime>
#include <QList>
#include <QMap>

#include "core/task.h"

class QTimer;

namespace schedule {

class TasksInvoker : public QObject
{
  Q_OBJECT
public:
  explicit TasksInvoker(QObject *parent = 0);

signals:
  void dateChanged(const QDate& date);
  void completed(const TaskPtr& task);
  void done();

public slots:
  void setDailyTasks(const QList<TaskPtr>& tasks);

  void start();
  void stop();

private slots:
  void timeoutHandler();

private:
  QTimer* timer_;
  QDate today_;
  QMultiMap<QTime, TaskPtr> tasks_;
};

} // namespace schedule

#endif // SCHEDULE_TASKS_INVOKER_H
