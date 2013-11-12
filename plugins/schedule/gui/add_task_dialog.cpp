#include <QLocale>
#include "settings_keys.h"
#include "add_task_dialog.h"
#include "ui_add_task_dialog.h"

AddTaskDialog::AddTaskDialog(QWidget* parent)
  : CenteredDialog(parent), ui(new Ui::AddTaskDialog) {
  ui->setupUi(this);
  ui->date_edit->setDisplayFormat(QLocale::system().dateFormat(QLocale::ShortFormat));
  ui->date_edit->setDate(QDate::currentDate());
  ui->time_edit->setDisplayFormat(GetDefaultValue(OPT_TIME_FORMAT).toString());
  ui->time_edit->setTime(QTime::currentTime());
}

AddTaskDialog::~AddTaskDialog() {
  delete ui;
}

void AddTaskDialog::accept() {
  Task new_task;
  new_task.date = ui->date_edit->date();
  new_task.time = ui->time_edit->time();
  new_task.description = ui->desc_text_edit->toPlainText();
  emit TaskAdded(new_task);
  CenteredDialog::accept();
}
