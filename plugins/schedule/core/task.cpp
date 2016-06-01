#include "task.h"

namespace schedule {

Task::Task() : id_(0)
{
}

int Task::id() const
{
  return id_;
}

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

void Task::setId(const int id)
{
  Q_ASSERT(id_ <= 0);
  id_ = id;
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

bool Task::isValid() const
{
  return id() > 0 && date().isValid() && time().isValid() && !note().isEmpty();
}

} // namespace schedule
