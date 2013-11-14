#include "task_manager.h"

TaskManager::TaskManager(QObject* parent)
  : QObject(parent), current_date_(QDate::currentDate()) {
}

void TaskManager::AddTask(const Task& task) {
  int old_size = tasks_.size();
  tasks_[task.date][task.time] = task.description;
  if (tasks_.size() != old_size) emit DatesUpdated(tasks_.keys());
  if (task.date == current_date_) emit TasksUpdated(tasks_[task.date]);
}

void TaskManager::RemoveTask(const Task& task) {
  auto date_iter = tasks_.find(task.date);
  if (date_iter == tasks_.end()) return;

  date_iter.value().remove(task.time);
  if (date_iter.value().empty()) {
    tasks_.erase(date_iter);
    emit DatesUpdated(tasks_.keys());
  }
  if (task.date == current_date_) emit TasksUpdated(tasks_[task.date]);
}

void TaskManager::SetCurrentDate(const QDate& date) {
  if (date == current_date_) return;
  current_date_ = date;
  emit TasksUpdated(tasks_[date]);
}

void TaskManager::SaveTasks() {
}

void TaskManager::LoadTasks() {
}

void TaskManager::CheckTime(const QDateTime& time) {
  auto date_iter = tasks_.find(time.date());
  if (date_iter == tasks_.end()) return;

  auto time_iter = date_iter.value().find(time.time());
  if (time_iter == date_iter.value().end()) return;

  emit TaskFound(time_iter.value());
  date_iter.value().erase(time_iter);
  if (date_iter.value().empty()) tasks_.erase(date_iter);
}
