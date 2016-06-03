#ifndef SCHEDULE_TASK_H
#define SCHEDULE_TASK_H

#include <QString>
#include <QDateTime>
#include <QSharedPointer>

namespace schedule {

class Task
{
public:
  Task();

  int id() const;

  QDate date() const;
  QTime time() const;

  QString note() const;

  void setId(const int id);

  void setDate(const QDate& dt);
  void setTime(const QTime& tm);

  void setNote(const QString& msg);

  bool isValid() const;

private:
  int id_;
  QDateTime date_time_;
  QString message_;
};

typedef QSharedPointer<Task> TaskPtr;

} // namespace schedule

#endif // SCHEDULE_TASK_H
