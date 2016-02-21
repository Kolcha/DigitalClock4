#ifndef SCHEDULE_ADD_TASK_DIALOG_H
#define SCHEDULE_ADD_TASK_DIALOG_H

#include <QDialog>
#include "../core/task.h"

namespace schedule {

namespace Ui {
class AddTaskDialog;
}

class AddTaskDialog : public QDialog {
  Q_OBJECT

public:
  explicit AddTaskDialog(Qt::DateFormat date_format, QWidget* parent = 0);
  ~AddTaskDialog();

signals:
  void TaskAdded(const Task& task);

public slots:
  void accept();

private:
  Ui::AddTaskDialog* ui;
};

} // namespace schedule

#endif // SCHEDULE_ADD_TASK_DIALOG_H
