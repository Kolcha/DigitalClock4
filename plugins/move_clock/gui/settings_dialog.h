/*
    Digital Clock: move clock plugin
    Copyright (C) 2014-2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef MOVE_CLOCK_SETTINGS_DIALOG_H
#define MOVE_CLOCK_SETTINGS_DIALOG_H

#include <QDialog>

namespace move_clock {

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
  void on_x0_value_valueChanged(int arg1);
  void on_kx_value_valueChanged(double arg1);
  void on_ax_value_valueChanged(int arg1);
  void on_nx_value_valueChanged(double arg1);
  void on_y0_value_valueChanged(int arg1);
  void on_ky_value_valueChanged(double arg1);
  void on_ay_value_valueChanged(int arg1);
  void on_ny_value_valueChanged(double arg1);
  void on_speed_slider_valueChanged(int value);

private:
  Ui::SettingsDialog* ui;
};


} // namespace move_clock

#endif // MOVE_CLOCK_SETTINGS_DIALOG_H
