#ifndef SCHEDULE_TASK_EDIT_DIALOG_H
#define SCHEDULE_TASK_EDIT_DIALOG_H

#include <QDialog>

namespace schedule {

namespace Ui {
class TaskEditDialog;
}

class TaskEditDialog : public QDialog
{
  Q_OBJECT

public:
  explicit TaskEditDialog(QWidget *parent = 0);
  ~TaskEditDialog();

  QDate date() const;
  QTime time() const;

  QString note() const;


  void setDate(const QDate& dt);
  void setTime(const QTime& tm);

  void setNote(const QString& nt);

private:
  Ui::TaskEditDialog *ui;
};

} // namespace schedule

#endif // SCHEDULE_TASK_EDIT_DIALOG_H
