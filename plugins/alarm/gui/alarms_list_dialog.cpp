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

#include "alarms_list_dialog.h"
#include "ui_alarms_list_dialog.h"

#include <algorithm>

#include <QDir>
#include <QFileInfo>

#include "core/alarm_item.h"

#include "edit_alarm_dialog.h"
#include "alarm_list_item_widget.h"

namespace alarm_plugin {

AlarmsListDialog::AlarmsListDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::AlarmsListDialog)
{
  setAttribute(Qt::WA_DeleteOnClose);
  ui->setupUi(this);
  connect(ui->adv_settings_btn, &QToolButton::clicked, this, &AlarmsListDialog::settingsButtonClicked);
  last_media_path_ = QDir::homePath();
}

AlarmsListDialog::~AlarmsListDialog()
{
  delete ui;
}

QList<AlarmItem*> AlarmsListDialog::alarmsList() const
{
  return alarms_;
}

void AlarmsListDialog::setAlarmsList(const QList<AlarmItem*>& alarms_list)
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

void AlarmsListDialog::setLastMediaPath(const QString& last_path)
{
  last_media_path_ = last_path;
  emit lastMediaPathChanged(last_media_path_);
}

void AlarmsListDialog::on_add_btn_clicked()
{
  AlarmItem* alarm = new AlarmItem();
  EditAlarmDialog dlg(alarm, this);
  dlg.setLastMediaPath(last_media_path_);
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
    if (alarm->media().isLocalFile())
      setLastMediaPath(QFileInfo(alarm->media().toLocalFile()).absolutePath());
    emit alarmAdded(alarm);
  } else {
    delete alarm;
  }
}

void AlarmsListDialog::on_del_btn_clicked()
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

void AlarmsListDialog::on_disable_all_btn_clicked()
{
  for (auto& alarm : alarms_) alarm->setEnabled(false);
}

void AlarmsListDialog::on_delete_all_btn_clicked()
{
  ui->alarms_list->clear();
  for (auto& alarm : alarms_) {
    emit alarmRemoved(alarm);
  }
  alarms_.clear();
}

} // namespace alarm_plugin
