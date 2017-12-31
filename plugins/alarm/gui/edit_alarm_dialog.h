/*
    Digital Clock: alarm plugin
    Copyright (C) 2017-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef ALARM_PLUGIN_EDIT_ALARM_DIALOG_H
#define ALARM_PLUGIN_EDIT_ALARM_DIALOG_H

#include <QDialog>

#include <QMap>
#include <QMediaPlayer>

class QCheckBox;

namespace alarm_plugin {

class AlarmItem;

namespace Ui {
class EditAlarmDialog;
}

class EditAlarmDialog : public QDialog
{
  Q_OBJECT

public:
  explicit EditAlarmDialog(AlarmItem* alarm_item, QWidget* parent = nullptr);
  ~EditAlarmDialog();

public slots:
  void reject() override;

  void setLastMediaPath(const QString& last_path);

private slots:
  void onAnyDayCheckboxToggled(bool checked);
  void onPlayerStateChanged(QMediaPlayer::State new_state);

  void on_filepath_edit_textChanged(const QString& arg1);
  void on_browse_btn_clicked();

private:
  Ui::EditAlarmDialog* ui;
  QString last_media_path_;
  AlarmItem* alarm_item_;
  AlarmItem* orig_item_;
  QMap<QCheckBox*, Qt::DayOfWeek> days_boxes_;
  QMediaPlayer* player_;
};

} // namespace alarm_plugin

#endif // ALARM_PLUGIN_EDIT_ALARM_DIALOG_H
