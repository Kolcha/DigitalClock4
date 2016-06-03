#ifndef SCHEDULE_SCHEDULE_DIALOG_H
#define SCHEDULE_SCHEDULE_DIALOG_H

#include <QDialog>

#include <QList>

#include "core/task.h"

class QSortFilterProxyModel;

namespace schedule {

class DailyTasksProvider;

namespace Ui {
class ScheduleDialog;
}

class ScheduleDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ScheduleDialog(QWidget *parent = 0);
  ~ScheduleDialog();

signals:
  void dateChanged(const QDate& dt);
  void taskCreated(const TaskPtr& tsk);
  void taskDeleted(const TaskPtr& tsk);

public slots:
  void setDates(const QList<QDate>& dates);
  void setTasks(const QList<TaskPtr>& tasks);

private slots:
  void on_dates_box_currentIndexChanged(int index);
  void on_add_btn_clicked();
  void on_del_btn_clicked();

  void on_edit_btn_clicked();

private:
  Ui::ScheduleDialog *ui;
  DailyTasksProvider* tasks_model_;
  QSortFilterProxyModel* sort_model_;
};

} // namespace schedule

#endif // SCHEDULE_SCHEDULE_DIALOG_H
