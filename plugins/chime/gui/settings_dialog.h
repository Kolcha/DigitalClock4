/*
    Digital Clock: chime plugin
    Copyright (C) 2016-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef CHIME_SETTINGS_DIALOG_H
#define CHIME_SETTINGS_DIALOG_H

#include <QDialog>

#include <QSettings>

namespace chime {

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SettingsDialog(QWidget* parent = 0);
  ~SettingsDialog();

  void Init(const QSettings::SettingsMap& settings);

signals:
  void OptionChanged(const QString& key, const QVariant& value);

private slots:
  void on_hour_enabled_cb_clicked(bool checked);
  void on_hour_count_box_activated(int index);
  void on_hour_sound_btn_clicked();

  void on_quarter_enabled_cb_clicked(bool checked);
  void on_quarter_count_box_activated(int index);
  void on_quarter_sound_btn_clicked();

  void on_quiet_hours_enabled_clicked(bool checked);
  void on_q_time_start_timeChanged(const QTime& time);
  void on_q_time_end_timeChanged(const QTime& time);

private:
  Ui::SettingsDialog* ui;
  QSettings::SettingsMap settings_;
};


} // namespace chime
#endif // CHIME_SETTINGS_DIALOG_H
