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

#include "settings_dialog.h"
#include "ui_settings_dialog.h"

#include <algorithm>

#include "core/alarm_item.h"

#include "edit_alarm_dialog.h"
#include "alarm_list_item_widget.h"

namespace alarm_plugin {

SettingsDialog::SettingsDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::SettingsDialog)
{
  setAttribute(Qt::WA_DeleteOnClose);
  ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
  delete ui;
}

QList<AlarmItem*> SettingsDialog::alarmsList() const
{
  return alarms_;
}

void SettingsDialog::setAlarmsList(const QList<AlarmItem*>& alarms_list)
{
  alarms_ = alarms_list;
  std::sort(alarms_.begin(), alarms_.end(), AlarmItem::timeCompare);
  ui->alarms_list->clear();
  for (auto& alarm : alarms_) {
    QListWidgetItem* item = new QListWidgetItem();
    AlarmListItemWidget* widget = new AlarmListItemWidget(alarm, ui->alarms_list);
    widget->setChecked(alarm->isEnabled());
    widget->setTime(alarm->time());
    widget->setDays(alarm->days());
    item->setSizeHint(widget->sizeHint());
    ui->alarms_list->addItem(item);
    ui->alarms_list->setItemWidget(item, widget);
  }
}

void SettingsDialog::on_add_btn_clicked()
{
  AlarmItem* alarm = new AlarmItem();
  EditAlarmDialog dlg(alarm, this);
  dlg.setWindowModality(Qt::WindowModal);
  if (dlg.exec() == QDialog::Accepted) {
    auto ins_iter = std::find_if(alarms_.begin(), alarms_.end(),
                                 [&] (const AlarmItem* i) -> bool { return alarm->time() < i->time(); });
    auto item_iter = alarms_.insert(ins_iter, alarm);
    QListWidgetItem* item = new QListWidgetItem();
    AlarmListItemWidget* widget = new AlarmListItemWidget(alarm, ui->alarms_list);
    widget->setChecked(alarm->isEnabled());
    widget->setTime(alarm->time());
    widget->setDays(alarm->days());
    item->setSizeHint(widget->sizeHint());
    ui->alarms_list->insertItem(std::distance(alarms_.begin(), item_iter), item);
    ui->alarms_list->setItemWidget(item, widget);
    emit alarmAdded(alarm);
  } else {
    delete alarm;
  }
}

void SettingsDialog::on_del_btn_clicked()
{
  QList<QListWidgetItem*> selected_items = ui->alarms_list->selectedItems();
  for (auto& item : selected_items) {
    int row = ui->alarms_list->row(item);
    AlarmItem* alarm = alarms_[row];
    alarms_.removeAt(row);
    emit alarmRemoved(alarm);
    QListWidgetItem* t_item = ui->alarms_list->takeItem(row);
    delete t_item;
  }
}

void SettingsDialog::on_disable_all_btn_clicked()
{
  for (auto& alarm : alarms_) alarm->setEnabled(false);
}

void SettingsDialog::on_delete_all_btn_clicked()
{
  ui->alarms_list->clear();
  for (auto& alarm : alarms_) {
    emit alarmRemoved(alarm);
  }
  alarms_.clear();
}

} // namespace alarm_plugin
