#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <QMap>
#include "task.h"

namespace schedule {

class TaskManager : public QObject {
  Q_OBJECT

public:
  explicit TaskManager(QObject* parent = 0);

signals:
  void DatesUpdated(const QList<QDate>& dates);
  void TasksUpdated(const QMap<QTime, QString>& tasks);
  void TaskFound(const QString& description);

public slots:
  void AddTask(const Task& task);
  void RemoveTask(const Task& task);
  void SetCurrentDate(const QDate& date);

  void SaveTasks();
  void LoadTasks();

  void CheckTime(const QDateTime& time, bool delete_task);

private:
  QMap<QDate, QMap<QTime, QString> > tasks_;
  QDate current_date_;
};

} // namespace schedule

#endif // TASK_MANAGER_H
