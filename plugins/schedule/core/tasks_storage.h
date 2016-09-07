#ifndef SCHEDULE_TASKS_STORAGE_H
#define SCHEDULE_TASKS_STORAGE_H

#include "settings_storage_wrapper.h"

#include <QList>

#include "core/task.h"

namespace schedule {

class TasksStorage : public SettingsStorageWrapper
{
  Q_OBJECT
public:
  explicit TasksStorage(SettingsStorage* backend, QObject* parent = 0);

signals:
  void datesLoaded(const QList<QDate>& dates);
  void tasksLoaded(const QList<TaskPtr>& lst);

public slots:
  void loadDates();
  void LoadTasks(const QDate& dt);

  void addTask(const TaskPtr& task);
  void delTask(const TaskPtr& task);

  void commit();      // TODO: replace with Wrapper::Accept()
  void reject();      // TODO: replace with Wrapper::Reject()

private:
  QList<QDate> listDates() const;
};

} // namespace schedule

#endif // SCHEDULE_TASKS_STORAGE_H
