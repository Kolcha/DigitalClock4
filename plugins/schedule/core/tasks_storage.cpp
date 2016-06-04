#include "tasks_storage.h"

#include "settings_storage.h"

namespace schedule {

TasksStorage::TasksStorage(SettingsStorage* backend, QObject *parent) : SettingsStorageWrapper(backend, parent)
{
}

void TasksStorage::loadDates()
{
  emit datesLoaded(listDates());
}

void TasksStorage::LoadTasks(const QDate& dt)
{
  QList<TaskPtr> tasks;
  QString date_key = QString("plugins/schedule/tasks/%1").arg(dt.toString("dd-MM-yyyy"));
  QStringList items = this->GetBackend()->ListChildren(date_key);
  for (auto& i : items) {
    bool id_ok = false;
    int id = i.toInt(&id_ok);
    if (!id_ok) continue;
    QString task_key = QString("%1/%2").arg(date_key).arg(id);
    TaskPtr task(new Task());
    task->setId(id);
    task->setDate(dt);
    task->setTime(this->getValue(QString("%1/%2").arg(task_key, "time")).toTime());
    task->setNote(this->getValue(QString("%1/%2").arg(task_key, "note")).toString());
    if (task->isValid()) tasks.append(task);
  }
  emit tasksLoaded(tasks);
}

void TasksStorage::addTask(const TaskPtr& task)
{
  if (task->note().isEmpty()) return;

  QString date_key = QString("plugins/schedule/tasks/%1").arg(task->date().toString("dd-MM-yyyy"));
  QStringList items = this->GetBackend()->ListChildren(date_key);
  std::sort(items.begin(), items.end());
  task->setId(items.isEmpty() ? 1 : items.back().toInt() + 1);

  int old_count = listDates().size();

  QString task_key = QString("%1/%2").arg(date_key).arg(task->id());
  this->setValue(QString("%1/%2").arg(task_key, "time"), task->time());
  this->setValue(QString("%1/%2").arg(task_key, "note"), task->note());

  QList<QDate> new_dates = listDates();
  if (new_dates.size() != old_count) emit datesLoaded(new_dates);
}

void TasksStorage::delTask(const TaskPtr& task)
{
  QString date_key = QString("plugins/schedule/tasks/%1").arg(task->date().toString("dd-MM-yyyy"));
  QString task_key = QString("%1/%2").arg(date_key).arg(task->id());
  this->remove(task_key);
  if (GetBackend()->ListChildren(date_key).isEmpty()) {
    this->remove(date_key);
  }
}

void TasksStorage::commit()
{
  this->Accept();
}

void TasksStorage::reject()
{
  this->Reject();
}

QList<QDate> TasksStorage::listDates() const
{
  QList<QDate> dates;
  QStringList items = this->GetBackend()->ListChildren("plugins/schedule/tasks");
  for (auto& i : items) {
    QDate dt = QDate::fromString(i, "dd-MM-yyyy");
    if (!dt.isValid()) continue;
    dates.append(dt);
  }
  return dates;
}

} // namespace schedule