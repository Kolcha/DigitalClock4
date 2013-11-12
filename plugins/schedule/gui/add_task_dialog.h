#ifndef ADD_TASK_DIALOG_H
#define ADD_TASK_DIALOG_H

#include "centered_dialog.h"

namespace Ui {
class AddTaskDialog;
}

class AddTaskDialog : public CenteredDialog {
  Q_OBJECT

public:
  explicit AddTaskDialog(QWidget* parent = 0);
  ~AddTaskDialog();

private:
  Ui::AddTaskDialog* ui;
};

#endif // ADD_TASK_DIALOG_H
