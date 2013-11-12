#include "add_task_dialog.h"
#include "ui_add_task_dialog.h"

AddTaskDialog::AddTaskDialog(QWidget* parent)
  : CenteredDialog(parent), ui(new Ui::AddTaskDialog) {
  ui->setupUi(this);
  setWindowIcon(QIcon(":/schedule.svg"));
}

AddTaskDialog::~AddTaskDialog() {
  delete ui;
}
