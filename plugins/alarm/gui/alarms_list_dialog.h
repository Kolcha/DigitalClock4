/*
    Digital Clock: alarm plugin
    Copyright (C) 2013-2017  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ALARM_ALARMS_LIST_DIALOG_H
#define ALARM_ALARMS_LIST_DIALOG_H

#include <QDialog>

#include <QList>

namespace alarm_plugin {

class AlarmItem;

namespace Ui {
class AlarmsListDialog;
}

class AlarmsListDialog : public QDialog
{
  Q_OBJECT

public:
  explicit AlarmsListDialog(QWidget* parent = 0);
  ~AlarmsListDialog();

  QList<AlarmItem*> alarmsList() const;

signals:
  void alarmAdded(AlarmItem* alarm);
  void alarmRemoved(AlarmItem* alarm);

public slots:
  void setAlarmsList(const QList<AlarmItem*>& alarms_list);

private slots:
  void on_add_btn_clicked();
  void on_del_btn_clicked();
  void on_disable_all_btn_clicked();
  void on_delete_all_btn_clicked();

private:
  Ui::AlarmsListDialog* ui;
  QList<AlarmItem*> alarms_;
};

} // namespace alarm_plugin

#endif // ALARM_ALARMS_LIST_DIALOG_H
