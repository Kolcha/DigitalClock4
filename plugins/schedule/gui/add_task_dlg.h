#ifndef ADD_TASK_DLG_H
#define ADD_TASK_DLG_H

#include "centered_dialog.h"
#include "../core/task.h"

namespace Ui {
class AddTaskDlg;
}

class AddTaskDlg : public CenteredDialog {
  Q_OBJECT

public:
  AddTaskDlg(Qt::DateFormat date_format, QWidget* parent = 0);
  ~AddTaskDlg();

signals:
  void TaskAdded(const Task& task);

public slots:
  void accept();

private:
  Ui::AddTaskDlg* ui;
};

#endif // ADD_TASK_DLG_H
