/*
    Digital Clock: alarm plugin
    Copyright (C) 2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "alarm_list_item_widget.h"
#include "ui_alarm_list_item_widget.h"

#include <algorithm>
#include <vector>

#include <QDate>
#include <QLocale>

#include "core/alarm_item.h"
#include "edit_alarm_dialog.h"

namespace alarm_plugin {

AlarmListItemWidget::AlarmListItemWidget(AlarmItem* item, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::AlarmListItemWidget),
  item_(item)
{
  ui->setupUi(this);

  std::vector<Qt::DayOfWeek> days(7);
  days[0] = Qt::Monday;
  days[1] = Qt::Tuesday;
  days[2] = Qt::Wednesday;
  days[3] = Qt::Thursday;
  days[4] = Qt::Friday;
  days[5] = Qt::Saturday;
  days[6] = Qt::Sunday;

  std::vector<QLabel*> labels(7);
  labels[0] = ui->day_1;
  labels[1] = ui->day_2;
  labels[2] = ui->day_3;
  labels[3] = ui->day_4;
  labels[4] = ui->day_5;
  labels[5] = ui->day_6;
  labels[6] = ui->day_7;

  auto day_iter = std::find(days.begin(), days.end(), QLocale::system().firstDayOfWeek());
  QDate d(2017, 2, 7);
  d = d.addDays(std::distance(days.begin(), day_iter) - 1);
  Q_ASSERT(day_iter != days.end());
  for (auto& label : labels) {
    label->setText(d.toString("ddd"));
    day_labels_[*day_iter] = label;
    d = d.addDays(1);
    ++day_iter;
    if (day_iter == days.end()) day_iter = days.begin();
  }

  setChecked(item->isEnabled());
  setTime(item->time());
  setDays(item->days());

  connect(ui->time_enabled, &QCheckBox::clicked, item, &AlarmItem::setEnabled);
  connect(item, &AlarmItem::toggled, ui->time_enabled, &QCheckBox::setChecked);
  connect(item, &AlarmItem::timeChanged, this, &AlarmListItemWidget::setTime);
  connect(item, &AlarmItem::daysChanged, this, &AlarmListItemWidget::setDays);
}

AlarmListItemWidget::~AlarmListItemWidget()
{
  delete ui;
}

bool AlarmListItemWidget::isChecked() const
{
  return ui->time_enabled->isChecked();
}

QTime AlarmListItemWidget::time() const
{
  return time_;
}

QSet<Qt::DayOfWeek> AlarmListItemWidget::days() const
{
  return days_;
}

void AlarmListItemWidget::setChecked(bool checked)
{
  ui->time_enabled->setChecked(checked);
}

void AlarmListItemWidget::setTime(const QTime& tm)
{
  time_ = tm;
  ui->time_enabled->setText(tm.toString(Qt::SystemLocaleShortDate));
}

void AlarmListItemWidget::setDays(const QSet<Qt::DayOfWeek>& days)
{
  days_ = days;
  for (auto iter = day_labels_.begin(); iter != day_labels_.end(); ++iter) {
    iter.value()->setEnabled(days.contains(iter.key()));
  }
}

void AlarmListItemWidget::on_edit_btn_clicked()
{
  EditAlarmDialog dlg(item_, this->window());
  dlg.setWindowModality(Qt::WindowModal);
  dlg.exec();
}

} // namespace alarm_plugin
