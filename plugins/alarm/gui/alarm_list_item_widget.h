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

#ifndef ALARM_PLUGIN_ALARM_LIST_ITEM_WIDGET_H
#define ALARM_PLUGIN_ALARM_LIST_ITEM_WIDGET_H

#include <QWidget>

#include <QMap>
#include <QSet>
#include <QTime>

class QLabel;

namespace alarm_plugin {

class AlarmItem;

namespace Ui {
class AlarmListItemWidget;
}

class AlarmListItemWidget : public QWidget
{
  Q_OBJECT

public:
  explicit AlarmListItemWidget(AlarmItem* item, QWidget* parent = nullptr);
  ~AlarmListItemWidget();

  bool isChecked() const;
  QTime time() const;
  QSet<Qt::DayOfWeek> days() const;

public slots:
  void setChecked(bool checked);
  void setTime(const QTime& tm);
  void setDays(const QSet<Qt::DayOfWeek>& days);

private slots:
  void on_edit_btn_clicked();

private:
  Ui::AlarmListItemWidget* ui;
  AlarmItem* item_;
  QMap<Qt::DayOfWeek, QLabel*> day_labels_;
  QTime time_;
  QSet<Qt::DayOfWeek> days_;
};

} // namespace alarm_plugin

#endif // ALARM_PLUGIN_ALARM_LIST_ITEM_WIDGET_H
