#ifndef SCHEDULE_DAILY_TASKS_PROVIDER_H
#define SCHEDULE_DAILY_TASKS_PROVIDER_H

#include <QAbstractTableModel>

#include <QList>

#include "core/task.h"

namespace schedule {

class DailyTasksProvider : public QAbstractTableModel
{
  Q_OBJECT

public:
  explicit DailyTasksProvider(QObject* parent = 0);

  // Header:
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

  // Basic functionality:
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  // Editable:
  bool setData(const QModelIndex& index, const QVariant& value,
               int role = Qt::EditRole) override;

  Qt::ItemFlags flags(const QModelIndex& index) const override;

  TaskPtr getTask(const QModelIndex& index) const;

public slots:
  void setTasks(const QList<TaskPtr>& lst);

private:
  QList<TaskPtr> tasks_;
};

} // namespace schedule

#endif // SCHEDULE_DAILY_TASKS_PROVIDER_H
