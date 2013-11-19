#ifndef SETTINGS_DLG_H
#define SETTINGS_DLG_H

#include "centered_dialog.h"
#include "../core/task.h"

namespace Ui {
class SettingsDlg;
}

class SettingsDlg : public CenteredDialog {
  Q_OBJECT

public:
  explicit SettingsDlg(QWidget* parent = 0);
  ~SettingsDlg();

signals:
  void TaskAdded(const Task& task);
  void TaskRemoved(const Task& task);
  void DateChanged(const QDate& new_date);
  void OptionChanged(const QString& key, const QVariant& value);

public slots:
  void SetDates(const QList<QDate>& dates);
  void SetTasks(const QMap<QTime, QString>& tasks);
  void SettingsListener(const QString& key, const QVariant& value);

private slots:
  void on_days_box_currentIndexChanged(int index);
  void on_add_btn_clicked();
  void on_del_btn_clicked();
  void on_long_format_toggled(bool checked);
  void on_autodelete_toggled(bool checked);

private:
  Ui::SettingsDlg* ui;
  Qt::DateFormat date_format_;
};

#endif // SETTINGS_DLG_H
