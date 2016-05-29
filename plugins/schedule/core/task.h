#ifndef SCHEDULE_TASK_H
#define SCHEDULE_TASK_H

#include <QString>
#include <QDateTime>
#include <QSharedPointer>

namespace schedule {

class Task
{
public:
  QDate date() const;
  QTime time() const;

  QString note() const;

  void setDate(const QDate& dt);
  void setTime(const QTime& tm);

  void setNote(const QString& msg);

private:
  QDateTime date_time_;
  QString message_;
};

typedef QSharedPointer<Task> TaskPtr;

} // namespace schedule

#endif // SCHEDULE_TASK_H
