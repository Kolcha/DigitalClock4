#include <QSettings>
#include <QStringList>
#include "task_manager.h"

#define APP_NAME   QString("Digital Clock")
#define ORG_NAME   QString("Nick Korotysh")
#define ROOT_KEY   QString("plugins/schedule/")

TaskManager::TaskManager(QObject* parent)
  : QObject(parent) {
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
  QSettings settings(ORG_NAME, APP_NAME);
  settings.remove(ROOT_KEY);
  for (auto d_iter = tasks_.begin(); d_iter != tasks_.end(); ++d_iter) {
    for (auto t_iter = d_iter.value().begin(); t_iter != d_iter.value().end(); ++t_iter) {
      QString date = d_iter.key().toString("dd-MM-yyyy");
      QString time = t_iter.key().toString("hh-mm");
      settings.setValue(ROOT_KEY + date + "/" + time, t_iter.value());
    }
  }
}

void TaskManager::LoadTasks() {
  QSettings settings(ORG_NAME, APP_NAME);
  settings.beginGroup(ROOT_KEY);
  QStringList dates = settings.childGroups();
  for (auto& date : dates) {
    QMap<QTime, QString>& date_tasks = tasks_[QDate::fromString(date, "dd-MM-yyyy")];
    settings.beginGroup(date);
    QStringList times = settings.childKeys();
    for (auto& time : times) {
      date_tasks[QTime::fromString(time, "hh-mm")] = settings.value(time).toString();
    }
    settings.endGroup();
  }
  emit DatesUpdated(tasks_.keys());
}

void TaskManager::CheckTime(const QDateTime& time) {
  auto date_iter = tasks_.find(time.date());
  if (date_iter == tasks_.end()) return;

  auto time_iter = date_iter.value().find(time.time());
  if (time_iter == date_iter.value().end()) return;

  emit TaskFound(time_iter.value());
  date_iter.value().erase(time_iter);
  if (date_iter.value().empty()) tasks_.erase(date_iter);
  SaveTasks();
}
