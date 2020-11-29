/*
    Digital Clock: schedule plugin
    Copyright (C) 2020  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef SCHEDULE_ADVANCED_SETTINGS_DIALOG_H
#define SCHEDULE_ADVANCED_SETTINGS_DIALOG_H

#include <QDialog>

#include <QSettings>

namespace schedule {

namespace Ui {
class AdvancedSettingsDialog;
}

class AdvancedSettingsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit AdvancedSettingsDialog(QWidget* parent = nullptr);
  ~AdvancedSettingsDialog();

  void Init(const QSettings::SettingsMap& settings);

signals:
  void OptionChanged(const QString& key, const QVariant& value);

private slots:
  void on_preview_btn_clicked();

  void on_msg_balloon_rbtn_clicked();
  void on_msg_dialog_rbtn_clicked();
  void on_msg_not_show_rbtn_clicked();

  void on_timeout_edit_valueChanged(int arg1);

  void on_play_sound_enabled_clicked(bool checked);
  void on_browse_btn_clicked();

  void on_show_tray_icon_clicked(bool checked);

  void on_hotkey_add_task_enabled_clicked(bool checked);
  void on_add_task_seq_edit_editingFinished();

private:
  Ui::AdvancedSettingsDialog* ui;
  QString curr_sound_file_;
};

} // namespace schedule

#endif // SCHEDULE_TASK_ADVANCED_SETTINGS_DIALOG_H
