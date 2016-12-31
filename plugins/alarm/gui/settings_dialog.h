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

#ifndef ALARM_SETTINGS_DIALOG_H
#define ALARM_SETTINGS_DIALOG_H

#include <QDialog>
#include <QMediaPlayer>

namespace alarm {

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SettingsDialog(QWidget* parent = 0);
  ~SettingsDialog();

public slots:
  void SettingsListener(const QString& key, const QVariant& value);

signals:
  void OptionChanged(const QString&, const QVariant&);

private slots:
  void PlayerStateChanged(QMediaPlayer::State state);
  void on_time_edit_timeChanged(const QTime& time);
  void on_alarm_enabled_toggled(bool checked);
  void on_browse_btn_clicked();
  void on_notification_enabled_toggled(bool checked);
  void on_message_edit_textChanged();
  void on_st_file_clicked();
  void on_st_stream_clicked();
  void on_stream_url_edit_textEdited(const QString& arg1);
  void on_volume_slider_valueChanged(int value);
  void on_play_btn_clicked();

private:
  Ui::SettingsDialog* ui;
  QString last_file_path_;
  QMediaPlayer* player_;
};

} // namespace alarm

#endif // ALARM_SETTINGS_DIALOG_H
