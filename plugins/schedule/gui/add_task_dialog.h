#ifndef ADD_TASK_DIALOG_H
#define ADD_TASK_DIALOG_H

#include "centered_dialog.h"
#include "../core/task.h"

namespace Ui {
class AddTaskDialog;
}

class AddTaskDialog : public CenteredDialog {
  Q_OBJECT

public:
  explicit AddTaskDialog(QWidget* parent = 0);
  ~AddTaskDialog();

signals:
  void TaskAdded(const Task& task);

public slots:
  void accept();

private:
  Ui::AddTaskDialog* ui;
};

#endif // ADD_TASK_DIALOG_H
