#ifndef SCHEDULE_TASKS_STORAGE_H
#define SCHEDULE_TASKS_STORAGE_H

#include <QObject>

#include <QList>
#include <QMap>

#include "core/task.h"

namespace schedule {

class TasksStorage : public QObject
{
  Q_OBJECT
public:
  explicit TasksStorage(QObject *parent = 0);

signals:
  void datesLoaded(const QList<QDate>& dates);
  void tasksLoaded(const QList<TaskPtr>& lst);

public slots:
  void loadDates();
  void LoadTasks(const QDate& dt);

  void addTask(const TaskPtr& tsk);
  void delTask(const TaskPtr& tsk);

  void commit();
  void reject();

private:
  QMap<QDate, QList<TaskPtr> > permanent_;
  QMap<QDate, QList<TaskPtr> > runtime_;
};

} // namespace schedule

#endif // SCHEDULE_TASKS_STORAGE_H
