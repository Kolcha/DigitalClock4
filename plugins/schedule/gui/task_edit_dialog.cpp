#include "task_edit_dialog.h"
#include "ui_task_edit_dialog.h"

#include <QLocale>

namespace schedule {

TaskEditDialog::TaskEditDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::TaskEditDialog)
{
  ui->setupUi(this);

  ui->dateEdit->setDisplayFormat(QLocale::system().dateFormat(QLocale::LongFormat));
  ui->timeEdit->setDisplayFormat(QLocale::system().timeFormat(QLocale::ShortFormat));

  QDateTime now = QDateTime::currentDateTime();
  ui->dateEdit->setMinimumDate(now.date());
  ui->timeEdit->setMinimumTime(now.time());
}

TaskEditDialog::~TaskEditDialog()
{
  delete ui;
}

QDate TaskEditDialog::date() const
{
  return ui->dateEdit->date();
}

QTime TaskEditDialog::time() const
{
  return ui->timeEdit->time();
}

QString TaskEditDialog::note() const
{
  return ui->textEdit->toPlainText();
}

void TaskEditDialog::setDate(const QDate& dt)
{
  ui->dateEdit->setDate(dt);
}

void TaskEditDialog::setTime(const QTime& tm)
{
  ui->timeEdit->setTime(tm);
}

void TaskEditDialog::setNote(const QString& nt)
{
  ui->textEdit->setPlainText(nt);
}

} // namespace schedule
