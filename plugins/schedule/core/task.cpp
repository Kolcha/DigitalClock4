#include "task.h"

namespace schedule {

QDate Task::date() const
{
  return date_time_.date();
}

QTime Task::time() const
{
  return date_time_.time();
}

QString Task::note() const
{
  return message_;
}

void Task::setDate(const QDate& dt)
{
  date_time_.setDate(dt);
}

void Task::setTime(const QTime& tm)
{
  date_time_.setTime(tm);
}

void Task::setNote(const QString& msg)
{
  message_ = msg;
}

} // namespace schedule
