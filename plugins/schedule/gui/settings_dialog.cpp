#include "settings_keys.h"
#include "add_task_dialog.h"
#include "settings_dialog.h"
#include "ui_settings_dialog.h"

SettingsDialog::SettingsDialog(QWidget* parent)
  : CenteredDialog(parent), ui(new Ui::SettingsDialog) {
  ui->setupUi(this);
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

void SettingsDialog::SetDates(const QList<QDate>& dates) {
  ui->days_box->clear();
  for (auto& date : dates) {
    ui->days_box->addItem(date.toString(Qt::SystemLocaleShortDate), date);
  }
  ui->days_box->setCurrentText(QDate::currentDate().toString(Qt::SystemLocaleShortDate));
}

void SettingsDialog::SetTasks(const QMap<QTime, QString>& tasks) {
  ui->tasks_table->setRowCount(0);
  for (auto iter = tasks.begin(); iter != tasks.end(); ++iter) {
    int index = ui->tasks_table->rowCount();
    ui->tasks_table->setRowCount(index + 1);
    QTableWidgetItem* item1 = new QTableWidgetItem(
          iter.key().toString(GetDefaultValue(OPT_TIME_FORMAT).toString()));
    item1->setData(Qt::UserRole, iter.key());
    QTableWidgetItem* item2 = new QTableWidgetItem(iter.value());
    ui->tasks_table->setItem(index, 0, item1);
    ui->tasks_table->setItem(index, 1, item2);
  }
}

void SettingsDialog::on_days_box_currentIndexChanged(int index) {
  emit DateChanged(ui->days_box->itemData(index).value<QDate>());
}

void SettingsDialog::on_add_btn_clicked() {
  AddTaskDialog* dlg = new AddTaskDialog(this);
  connect(dlg, SIGNAL(TaskAdded(Task)), this, SIGNAL(TaskAdded(Task)));
  dlg->show();
}

void SettingsDialog::on_del_btn_clicked() {
  Task task;
  task.date = ui->days_box->itemData(ui->days_box->currentIndex()).value<QDate>();
  int row = ui->tasks_table->currentRow();
  task.time = ui->tasks_table->item(row, 0)->data(Qt::UserRole).value<QTime>();
  task.description = ui->tasks_table->item(row, 1)->text();
  emit TaskRemoved(task);
}
