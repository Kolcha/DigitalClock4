/*
    Digital Clock: schedule plugin
    Copyright (C) 2013-2017  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "tasks_invoker.h"

#include <QTimer>

namespace schedule {

TasksInvoker::TasksInvoker(QObject* parent) : QObject(parent)
{
  timer_ = new QTimer(this);
  timer_->setInterval(30000);
  timer_->setSingleShot(false);
  connect(timer_, &QTimer::timeout, this, &TasksInvoker::timeoutHandler);
  today_ = QDate::currentDate();
}

void TasksInvoker::setDailyTasks(const QList<TaskPtr>& tasks)
{
  if (!timer_->isActive()) return;
  tasks_.clear();
  for (auto& tsk : tasks) {
    tasks_.insert(tsk->time(), tsk);
  }
  Q_ASSERT(tasks_.size() == tasks.size());
  this->timeoutHandler();
}

void TasksInvoker::start()
{
  Q_ASSERT(!timer_->isActive());
  timer_->start();
  emit dateChanged(today_);
}

void TasksInvoker::stop()
{
  Q_ASSERT(timer_->isActive());
  timer_->stop();
  tasks_.clear();
}

void TasksInvoker::timeoutHandler()
{
  bool tasks_found = false;
  QDateTime now = QDateTime::currentDateTime();
  if (now.date() != today_) {
    for (auto iter = tasks_.begin(); iter != tasks_.end(); ++iter) {
      emit completed(iter.value());
      if (!tasks_found) tasks_found = true;
    }
    tasks_.clear();
    today_ = now.date();
    emit dateChanged(today_);
  } else {
    auto last_task_iter = tasks_.lowerBound(now.time());
    for (auto iter = tasks_.begin(); iter != last_task_iter;) {
      emit completed(iter.value());
      tasks_.erase(iter++);
      if (!tasks_found) tasks_found = true;
    }
  }
  if (tasks_found) emit done();
}

} // namespace schedule
