#include "settings_keys.h"
#include "../core/schedule_settings.h"
#include "add_task_dialog.h"
#include "settings_dialog.h"
#include "ui_settings_dialog.h"

namespace schedule {

SettingsDialog::SettingsDialog(QWidget* parent)
  : QDialog(parent), ui(new Ui::SettingsDialog),
    date_format_(Qt::SystemLocaleShortDate) {
  setAttribute(Qt::WA_DeleteOnClose);
  ui->setupUi(this);

  QHeaderView* header = ui->tasks_table->horizontalHeader();
  header->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  header->setSectionResizeMode(1, QHeaderView::Stretch);
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

void SettingsDialog::SetDates(const QList<QDate>& dates) {
  ui->days_box->clear();
  for (auto& date : dates) {
    ui->days_box->addItem(date.toString(date_format_), date);
  }
  ui->days_box->setCurrentText(QDate::currentDate().toString(date_format_));
  emit DateChanged(ui->days_box->currentData().toDate());
}

void SettingsDialog::SetTasks(const QMap<QTime, QString>& tasks) {
  ui->tasks_table->setRowCount(0);
  for (auto iter = tasks.begin(); iter != tasks.end(); ++iter) {
    int index = ui->tasks_table->rowCount();
    ui->tasks_table->setRowCount(index + 1);
    QTableWidgetItem* item1 = new QTableWidgetItem(
          iter.key().toString("hh:mm"));
    item1->setData(Qt::UserRole, iter.key());
    QTableWidgetItem* item2 = new QTableWidgetItem(iter.value());
    ui->tasks_table->setItem(index, 0, item1);
    ui->tasks_table->setItem(index, 1, item2);
  }
}

void SettingsDialog::SettingsListener(const QString& key, const QVariant& value) {
  if (key == OPT_DATE_FORMAT) {
    date_format_ = (Qt::DateFormat)(value.toInt());
    ui->long_format->setChecked(date_format_ == Qt::SystemLocaleLongDate);
  }
  if (key == OPT_TASK_DELETE) {
    ui->autodelete->setChecked(value.toBool());
  }
}

void SettingsDialog::on_days_box_currentIndexChanged(int index) {
  emit DateChanged(ui->days_box->itemData(index).toDate());
}

void SettingsDialog::on_add_btn_clicked() {
  AddTaskDialog* dlg = new AddTaskDialog(date_format_, this);
  connect(dlg, SIGNAL(TaskAdded(Task)), this, SIGNAL(TaskAdded(Task)));
  dlg->show();
}

void SettingsDialog::on_del_btn_clicked() {
  Task task;
  task.date = ui->days_box->itemData(ui->days_box->currentIndex()).toDate();
  int row = ui->tasks_table->currentRow();
  task.time = ui->tasks_table->item(row, 0)->data(Qt::UserRole).toTime();
  task.description = ui->tasks_table->item(row, 1)->text();
  emit TaskRemoved(task);
}

void SettingsDialog::on_long_format_toggled(bool checked) {
  date_format_ = checked ? Qt::SystemLocaleLongDate : Qt::SystemLocaleShortDate;
  emit OptionChanged(OPT_DATE_FORMAT, (int)date_format_);

  for (int i = 0; i < ui->days_box->count(); ++i) {
    QDate date = ui->days_box->itemData(i).toDate();
    ui->days_box->setItemText(i, date.toString(date_format_));
  }
}

void SettingsDialog::on_autodelete_toggled(bool checked) {
  emit OptionChanged(OPT_TASK_DELETE, checked);
}

} // namespace schedule
