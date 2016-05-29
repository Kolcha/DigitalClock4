#include "tasks_storage.h"

namespace schedule {

TasksStorage::TasksStorage(QObject *parent) : QObject(parent)
{
  QList<QDate> dates;
  QDate today = QDate::currentDate();
  dates.append(today);
  dates.append(today.addDays(1));
  dates.append(today.addDays(2));
  for (auto& dt : dates) {
    QList<TaskPtr>& tasks = permanent_[dt];
    for (int i = 0; i < 10; i++) {
      QTime now = QTime::currentTime();
      TaskPtr tsk(new Task());
      tsk->setDate(dt);
      tsk->setTime(now.addSecs(3600*(2+i) + 60*45));
      tsk->setNote(QString("task %1 %2").arg(tsk->date().toString(), tsk->time().toString()));
      tasks.append(tsk);
    }
  }
  reject();
}

void TasksStorage::loadDates()
{
  emit datesLoaded(runtime_.keys());
}

void TasksStorage::LoadTasks(const QDate& dt)
{
  emit tasksLoaded(runtime_[dt]);
}

void TasksStorage::addTask(const TaskPtr& tsk)
{
  int old_count = runtime_.size();
  runtime_[tsk->date()].append(tsk);
  if (runtime_.size() != old_count) emit datesLoaded(runtime_.keys());
}

void TasksStorage::delTask(const TaskPtr& tsk)
{
  auto dt_iter = runtime_.find(tsk->date());
  Q_ASSERT(dt_iter != runtime_.end());
  Q_ASSERT(!dt_iter.value().isEmpty());
  auto tsk_iter = std::find(dt_iter->begin(), dt_iter->end(), tsk);
  Q_ASSERT(tsk_iter != dt_iter->end());
  dt_iter->erase(tsk_iter);
  if (dt_iter.value().isEmpty()) {
    Q_ASSERT(dt_iter->isEmpty());
    runtime_.erase(dt_iter);
    emit datesLoaded(runtime_.keys());
  }
}

void TasksStorage::commit()
{
  permanent_.clear();
  for (auto di = runtime_.begin(); di != runtime_.end(); ++di) {
    QList<TaskPtr>& p_list = permanent_[di.key()];
    p_list.clear();
    for (auto& ti : di.value()) {
      TaskPtr tsk(new Task());
      *tsk = *ti;
      p_list.append(tsk);
    }
  }
}

void TasksStorage::reject()
{
  runtime_.clear();
  for (auto di = permanent_.begin(); di != permanent_.end(); ++di) {
    QList<TaskPtr>& r_list = runtime_[di.key()];
    r_list.clear();
    for (auto& ti : di.value()) {
      TaskPtr tsk(new Task());
      *tsk = *ti;
      r_list.append(tsk);
    }
  }
}

} // namespace schedule
