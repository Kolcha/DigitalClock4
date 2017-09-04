/*
    Digital Clock: countdown timer plugin
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

#ifndef COUNTDOWN_TIMER_SETTINGS_DIALOG_H
#define COUNTDOWN_TIMER_SETTINGS_DIALOG_H

#include <QDialog>
#include <QSettings>

namespace countdown_timer {

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SettingsDialog(QWidget* parent = nullptr);
  ~SettingsDialog();

  void AddCommonWidget(QWidget* widget);

  void Init(const QSettings::SettingsMap& settings);

signals:
  void OptionChanged(const QString&, const QVariant&);

private slots:
  void on_target_time_rbtn_clicked();
  void on_interval_rbtn_clicked();

  void on_target_time_edit_dateTimeChanged(const QDateTime &date_time);

  void on_h_edit_valueChanged(int arg1);
  void on_m_edit_valueChanged(int arg1);
  void on_s_edit_valueChanged(int arg1);

  void on_show_msg_clicked(bool checked);
  void on_msg_text_edit_textChanged();

private:
  Ui::SettingsDialog* ui;
};

} // namespace countdown_timer

#endif // COUNTDOWN_TIMER_SETTINGS_DIALOG_H
