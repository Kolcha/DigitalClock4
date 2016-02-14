#include <QLocale>
#include "settings_keys.h"
#include "add_task_dialog.h"
#include "ui_add_task_dialog.h"

namespace schedule {

AddTaskDialog::AddTaskDialog(Qt::DateFormat date_format, QWidget* parent)
  : QDialog(parent), ui(new Ui::AddTaskDialog) {
  setAttribute(Qt::WA_DeleteOnClose);
  ui->setupUi(this);
  QLocale::FormatType format_type =
      date_format == Qt::SystemLocaleLongDate ? QLocale::LongFormat : QLocale::ShortFormat;
  ui->date_edit->setDisplayFormat(QLocale::system().dateFormat(format_type));
  ui->date_edit->setDate(QDate::currentDate());
  ui->time_edit->setDisplayFormat("hh:mm");
  ui->time_edit->setTime(QTime::currentTime());
}

AddTaskDialog::~AddTaskDialog() {
  delete ui;
}

void AddTaskDialog::accept() {
  Task new_task;
  new_task.date = ui->date_edit->date();
  new_task.time = ui->time_edit->time();
  new_task.time = new_task.time.addSecs(-new_task.time.second()).addMSecs(-new_task.time.msec());
  new_task.description = ui->desc_text_edit->toPlainText();
  emit TaskAdded(new_task);
}

} // namespace schedule
